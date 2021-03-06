#include "CTP_TD.h"
#include "ITradeSpi.h"
#include "EnumTranslator.h"

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>
#include <boost/filesystem.hpp>
#include <boost/format.hpp>

#include <iostream>
#include <boost\asio.hpp>
#include <functional>
#include <thread>

namespace CTP
{
	CTP_TD::CTP_TD(const char* aConfigFile,AT::ITradeSpi* apTradeSpi)
		: m_RequestID(0)
		, m_OrderRef(0)
		, m_pTradeSpi(apTradeSpi)
		, m_ConfigFile(aConfigFile)
	{
		EnumTranslator::Init();
	}

	CTP_TD::~CTP_TD(void)
	{
		if(m_pTraderAPI)
		{
			m_pTraderAPI->RegisterSpi(NULL);
			m_pTraderAPI->Join();
		} 
		m_pTraderAPI->Release();

		m_pWorker.reset();

		if(m_pReplayThread->joinable())  m_pReplayThread->join();
	}


	void CTP_TD::Start()
	{

		m_pWorker.reset( new boost::asio::io_service::work(m_IO_Service));
		typedef std::size_t (boost::asio::io_service::*signature_type)();
		signature_type run_ptr = &boost::asio::io_service::run;
		m_pReplayThread.reset(new std::thread( std::bind(run_ptr,&m_IO_Service))) ;

		LoadConfigFromFile();

		m_pOrderTable.reset(new AT::AT_OrderTable(m_OrderTableDB.c_str()));

		using namespace boost::filesystem;
		path lWorkFlowPath(m_CTP_WorkFlowDir);
		if(!exists(lWorkFlowPath))
		{
			create_directory(lWorkFlowPath);
		}

		std::string lCreateDIr = lWorkFlowPath.string();
		lCreateDIr += "/";
		m_pTraderAPI = CThostFtdcTraderApi::CreateFtdcTraderApi(lCreateDIr.c_str());
		m_pTraderAPI->RegisterSpi(this);
		m_pTraderAPI->SubscribePublicTopic(THOST_TERT_RESUME);					
		m_pTraderAPI->SubscribePrivateTopic(THOST_TERT_RESUME);

		char buf_front[256];
		strcpy_s(buf_front,sizeof(buf_front),m_FrontAddress.c_str());
		m_pTraderAPI->RegisterFront(buf_front);
		m_pTraderAPI->Init();

		boost::format lInfo("CTP TD Start \n [FrontAddress: %s] \n [BrokerID: %s] \n  [User: %s] \n [WorkdFlowDir %s] \n");
		m_pTradeSpi->NotifyStateTD(AT::ETradeState::START,	str(lInfo % m_FrontAddress % m_BrokerID % m_UserID % m_CTP_WorkFlowDir).c_str());
	}

	void CTP_TD::LoadConfigFromFile()
	{
		boost::property_tree::ptree lpt;
		read_xml(m_ConfigFile,lpt);
		ATLOG(AT::LogLevel::L_ERROR ,m_ConfigFile);
		m_BrokerID  = lpt.get<std::string>("AccountFile.ExchangeAccountInfo.BrokeID");
		m_UserID  = lpt.get<std::string>("AccountFile.ExchangeAccountInfo.UserID");
		m_Password  = lpt.get<std::string>("AccountFile.ExchangeAccountInfo.Password");
		m_FrontAddress  = lpt.get<std::string>("AccountFile.ExchangeAccountInfo.Front");
		m_CTP_WorkFlowDir  = lpt.get<std::string>("AccountFile.ExchangeAccountInfo.WorkFlowDir");
		m_OrderTableDB = lpt.get<std::string>("AccountFile.ExchangeAccountInfo.OrderTablePath");
	}


	void CTP_TD::OnFrontConnected()
	{
		CThostFtdcReqUserLoginField lLoginReq;
		memset(&lLoginReq,0,sizeof(CThostFtdcReqUserLoginField));
		strcpy_s(lLoginReq.BrokerID,11,m_BrokerID.c_str());
		strcpy_s(lLoginReq.UserID,15,m_UserID.c_str());
		strcpy_s(lLoginReq.Password,41,m_Password.c_str());
		int ret = m_pTraderAPI->ReqUserLogin(&lLoginReq,++m_RequestID);

		if(ret!= 0) ATLOG(AT::LogLevel::L_ERROR ,"ReqUserLogin Send Failed");
	}

	void CTP_TD::OnRspUserLogin( CThostFtdcRspUserLoginField *apRspUserLogin, CThostFtdcRspInfoField *apRspInfo, int anRequestID, bool abIsLast )
	{
		if(IsErrorRspInfo(apRspInfo))
		{
			boost::format lLoginFailed( "Login Failed Message %s");
			m_pTradeSpi->NotifyStateTD(AT::ETradeState::STOP,str(lLoginFailed % apRspInfo->ErrorMsg).c_str());
			return;
		}
		else
		{
			m_SessionID = apRspUserLogin->SessionID;
			m_FrontID = apRspUserLogin->FrontID;
			//to check again
			m_OrderRef = std::stoi(apRspUserLogin->MaxOrderRef);
			CThostFtdcQrySettlementInfoConfirmField lQryFiled;
			memset(&lQryFiled,0,sizeof(lQryFiled));
			int ret = m_pTraderAPI->ReqQrySettlementInfoConfirm(&lQryFiled,++m_RequestID);
			if(ret!=0)  ATLOG(AT::LogLevel::L_ERROR ,"ReqQrySettlementInfoConfirm Send Failed");
		}
	}

	void CTP_TD::OnRspQrySettlementInfoConfirm( CThostFtdcSettlementInfoConfirmField *apSettlementInfoConfirm, CThostFtdcRspInfoField *apRspInfo, int nRequestID, bool bIsLast )
	{
		if(IsErrorRspInfo(apRspInfo))
		{
			boost::format lLoginFailed( "QrySettlementInfoConfirm Failed Message %s");
			m_pTradeSpi->NotifyStateTD(AT::ETradeState::STOP,str(lLoginFailed % apRspInfo->ErrorMsg).c_str());
			return;
		}
		else
		{
			if(apSettlementInfoConfirm!= NULL)
			{
				m_pTradeSpi->NotifyStateTD(AT::ETradeState::READY,"TD Ready");
			}
			else
			{
				CThostFtdcSettlementInfoConfirmField lQryFiled;
				memset(&lQryFiled,0,sizeof(lQryFiled));
				strcpy_s(lQryFiled.BrokerID,11,m_BrokerID.c_str());
				strcpy_s(lQryFiled.InvestorID,13,m_UserID.c_str());
				int ret = m_pTraderAPI->ReqSettlementInfoConfirm(&lQryFiled,++m_RequestID);
				if(ret!=0)  ATLOG(AT::LogLevel::L_ERROR ,"ReqQryExchange Send Failed"); 
			}
		}
	}

	void CTP_TD::OnRspSettlementInfoConfirm( CThostFtdcSettlementInfoConfirmField *pSettlementInfoConfirm, CThostFtdcRspInfoField *apRspInfo, int nRequestID, bool bIsLast )
	{
		if(!IsErrorRspInfo(apRspInfo))
		{
			m_pTradeSpi->NotifyStateTD(AT::ETradeState::READY,"TD Ready");
		}
		else
		{
			m_pTradeSpi->NotifyStateTD(AT::ETradeState::STOP,"TD SettlementInfoConfirm Failed");
		}
	}

	void CTP_TD::CreateOrder( const AT::InputOrder& aNewOrder )
	{

		//TODO check valid Order
		std::cout<<"Receive Order \n\n\n"<<ToString(aNewOrder);
		InputOrderTypePtr lExchangeOrder = BuildExchangeOrder(aNewOrder);
		int ret = m_pTraderAPI->ReqOrderInsert(lExchangeOrder.get(),++m_RequestID);
		if(ret!=0)
		{
			ATLOG(AT::LogLevel::L_ERROR,"CreateOrder  Failed");
			std::shared_ptr<AT::OrderUpdate> lRejectOrder (new AT::OrderUpdate);
			AT::CopyOrderFields(*lRejectOrder,aNewOrder);
			lRejectOrder->m_UpdateTime = AT::AT_Local_Time();
			lRejectOrder->m_OrderStatus = AT::OrderStatusType::Canceled;
			lRejectOrder->m_OrderSubmitState = AT::OrderSubmitStatusType::InsertRejected;
			lRejectOrder->m_TradedVol = 0;
			lRejectOrder->m_LiveVol = 0;
			m_IO_Service.post(std::bind(&CTP_TD::SendOrderUpdate,this,lRejectOrder));
		}
		else
		{
			std::shared_ptr<AT::OrderUpdate > lpOrder(new AT::OrderUpdate);
			AT::CopyOrderFields(*lpOrder,aNewOrder);
			std::string lPlatformOrderID = CTP::MakePlatformOrderID(lExchangeOrder->OrderRef,m_FrontID,m_SessionID);
			if(sizeof(lpOrder->m_PlatformOrderID) -1 < lPlatformOrderID.size())
			{
				ATLOG(AT::LogLevel::L_ERROR,"ExchangeOrderID is too long");
			}
			strcpy_s(lpOrder->m_PlatformOrderID ,sizeof(lpOrder->m_PlatformOrderID),lPlatformOrderID.c_str());
			m_pOrderTable->PutItem(lpOrder);
			ATLOG(AT::LogLevel::L_INFO,"Send CreateOrder  Succeed");
		}
	}

	void CTP_TD::OnFrontDisconnected( int nReason )
	{
		ATLOG(AT::LogLevel::L_ERROR,"Front Disconnect");
	}

	CTP::InputOrderTypePtr CTP_TD::BuildExchangeOrder( const AT::InputOrder& aNewOrder )
	{

		InputOrderTypePtr lRetPtr(new CThostFtdcInputOrderField);
		memset(lRetPtr.get(),0,sizeof(CThostFtdcInputOrderField));
		CThostFtdcInputOrderField& lRet = *lRetPtr;

		SetupAccountAndOthers(lRet, aNewOrder);
		SetupPrice_Vol(lRet,aNewOrder);
		SetupDirectionAndOpenClose(lRet,aNewOrder);
		return lRetPtr;
	}

	void CTP_TD::OnRspOrderInsert( CThostFtdcInputOrderField *pInputOrder, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast )
	{
		if(IsErrorRspInfo(pRspInfo))
		{
			std::string lPlatformOrderID = MakePlatformOrderID(pInputOrder->OrderRef,m_FrontID,m_SessionID);

			ATLOG(AT::LogLevel::L_ERROR,"CreateOrder  Failed");
			std::shared_ptr<AT::OrderUpdate> lRejectOrder = m_pOrderTable->FindOrderByPlatformOrderID(lPlatformOrderID);
			lRejectOrder->m_UpdateTime = AT::AT_Local_Time();
			lRejectOrder->m_OrderStatus  = AT::OrderStatusType::Canceled;//
			lRejectOrder->m_OrderSubmitState = AT::OrderSubmitStatusType::InsertRejected ;

			strcpy_s(lRejectOrder->m_ErrorMessage ,sizeof(lRejectOrder->m_ErrorMessage),pRspInfo->ErrorMsg);
			lRejectOrder->m_TradedVol = 0;
			lRejectOrder->m_LiveVol = 0;
			m_IO_Service.post(std::bind(&CTP_TD::SendOrderUpdate,this,lRejectOrder));
		}
		else
		{
			ATLOG(AT::LogLevel::L_ERROR,"Un handle OnRspOrderInsert");
			ATLOG(AT::LogLevel::L_ERROR,pRspInfo->ErrorMsg);
		}
	}

	void CTP_TD::OnRtnOrder( CThostFtdcOrderField *apExchangeOrder )
	{
		std::string lPlatformOrderID = MakePlatformOrderID(apExchangeOrder->OrderRef,apExchangeOrder->FrontID,apExchangeOrder->SessionID);
		std::shared_ptr<AT::OrderUpdate> lpOrder = m_pOrderTable->FindOrderByPlatformOrderID(lPlatformOrderID);
		if(!lpOrder)
			return;
		lpOrder->m_TradedVol = apExchangeOrder->VolumeTraded;
		lpOrder->m_LiveVol = apExchangeOrder->VolumeTotal;

		lpOrder->m_UpdateTime = AT::AT_Local_Time();// 
		lpOrder->m_OrderStatus  = EnumTranslator::TransOrderStatusType(apExchangeOrder->OrderStatus);
		lpOrder->m_OrderSubmitState = EnumTranslator::TransOrderSubmitStatusType(apExchangeOrder->OrderSubmitStatus);

		ATLOG(AT::LogLevel::L_INFO,ToString(*lpOrder));

		std::string lExchangeID = CTP::MakeExchangeOrderID(apExchangeOrder->ExchangeID,apExchangeOrder->OrderSysID);
		if(sizeof(lpOrder->m_ExchangeOrderID) -1 < lExchangeID.size())
		{
			ATLOG(AT::LogLevel::L_ERROR,"ExchangeOrderID is too long");
		}
		strcpy_s(lpOrder->m_ExchangeOrderID ,sizeof(lpOrder->m_ExchangeOrderID),lExchangeID.c_str());

		m_pOrderTable->PutItem(lpOrder);
		m_IO_Service.post(std::bind(&CTP_TD::SendOrderUpdate,this,lpOrder));
	}


	void CTP_TD::OnRtnTrade( CThostFtdcTradeField *apExchangeTrade )
	{
		std::string lExchangOrderID = CTP::MakeExchangeOrderID(apExchangeTrade->ExchangeID,apExchangeTrade->OrderSysID);
		std::shared_ptr<AT::OrderUpdate> lpOrder = m_pOrderTable->FindOrderByExchangeOrderID(lExchangOrderID);
		if(lpOrder)
		{
			std::shared_ptr<AT::TradeUpdate> lpTrade(new AT::TradeUpdate);
			memcpy(lpTrade->AccoutID, lpOrder->AccoutID,sizeof(lpTrade->AccoutID));
			memcpy(lpTrade->InstrumentID, lpOrder->InstrumentID,sizeof(lpTrade->InstrumentID));
			memcpy(lpTrade->m_TradeID,apExchangeTrade->TradeID,sizeof(lpTrade->m_TradeID));
			lpTrade-> m_TradePrice = AT::TranPriceToInt(apExchangeTrade->Price);
			lpTrade->m_TradeVol = apExchangeTrade->Volume;
			lpTrade-> m_TradeTime = AT::AT_Local_Time();
			lpTrade-> m_Key = lpOrder->m_Key;

	
			lpTrade->m_BuySellType = EnumTranslator::TransBuySellType(apExchangeTrade->Direction) ;
			lpTrade->m_OpenCloseType = EnumTranslator::TransOpenCloseType(apExchangeTrade->OffsetFlag);

			m_IO_Service.post(std::bind(&CTP_TD::SendTradeUpdate,this,lpTrade));
		}
	}

	void CTP_TD::OnRspOrderAction( CThostFtdcInputOrderActionField *pInputOrderAction, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast )
	{
		if(IsErrorRspInfo(pRspInfo))
		{
			//todo cancel reject should rsp outside...
			std::string lPlatformOrderID = MakePlatformOrderID(pInputOrderAction->OrderRef,pInputOrderAction->FrontID,pInputOrderAction->SessionID);
			auto lpOrder  = m_pOrderTable->FindOrderByPlatformOrderID(lPlatformOrderID);
			std::string lOrderKey = AT::ToString(lpOrder->m_Key);
			std::string lErrorInfo = "OnRspOrderAction with error ";
			lErrorInfo += lOrderKey;
			lErrorInfo += "  info " ;
			lErrorInfo += pRspInfo->ErrorMsg;
			ATLOG(AT::LogLevel::L_ERROR,lErrorInfo);
		}
		else
		{
			ATLOG(AT::LogLevel::L_ERROR,"Un handle Message OnRspOrderAction without error ");
		}
	}



	void CTP_TD::SendOrderUpdate( std::shared_ptr<AT::OrderUpdate> apOrderUpdate )
	{
		std::cout<< ToString(*apOrderUpdate);
		m_pTradeSpi->OnRtnOrder(*apOrderUpdate);
	}

	void CTP_TD::SendTradeUpdate( std::shared_ptr<AT::TradeUpdate > apTradeUpdate )
	{
		std::cout<< ToString(*apTradeUpdate);
		m_pTradeSpi->OnRtnTrade(*apTradeUpdate);
	}

	void CTP_TD::DeleteOrder( const AT::CancelOrder& aDelOrder )
	{
		AT::AT_Order_Key lOrderKey = aDelOrder.m_Key;
		std::shared_ptr<AT::OrderUpdate> lpOrder = m_pOrderTable->GetItem(lOrderKey);

		if(!lpOrder)
		{
			std::string logInfo = "Del Order did not Exist in Cache: ";
			logInfo += AT::ToString(lOrderKey);
			ATLOG(AT::LogLevel::L_ERROR,logInfo);
			return;
		}
			int lFrontID ;
			int lSessionID;
			std::string lOrderRef = ResolveThostOrderID(lpOrder->m_PlatformOrderID,lSessionID,lFrontID);

			CThostFtdcInputOrderActionField  lOrderAction ;
			memset(&lOrderAction,0,sizeof(lOrderAction));
			lOrderAction.ActionFlag =THOST_FTDC_AF_Delete;
			strcpy_s(lOrderAction.BrokerID ,sizeof(lOrderAction.BrokerID),m_BrokerID.c_str());
			lOrderAction.FrontID =lFrontID;
			lOrderAction.SessionID = lSessionID;
			strcpy_s(lOrderAction.InstrumentID ,sizeof(lOrderAction.InstrumentID) ,lpOrder->InstrumentID);
			strcpy_s(lOrderAction.InvestorID ,sizeof(lOrderAction.InvestorID),m_UserID.c_str());
			strcpy_s(lOrderAction.OrderRef ,sizeof(lOrderAction.OrderRef) ,lOrderRef.c_str());
			strcpy_s(lOrderAction.UserID ,sizeof(lOrderAction.UserID) ,m_UserID.c_str());
			int ret = m_pTraderAPI->ReqOrderAction(&lOrderAction,++m_RequestID);
			if(ret != 0) ATLOG(AT::LogLevel::L_ERROR,"DeleteOrder Send Failed");
	}

	void CTP_TD::ModifyOrder( const AT::ModifyOrder& aRequest )
	{
		//todo 
	}

	void CTP_TD::Stop()
	{

	}


	void CTP_TD::SetupPrice_Vol( CThostFtdcInputOrderField &lRet,const AT::InputOrder& aNewOrder )
	{

		lRet.OrderPriceType = EnumTranslator::TransOrderPriceType(aNewOrder.m_OrderPriceType);
		lRet.LimitPrice = AT::TransPriceToDouble(aNewOrder.m_Price);
		lRet.TimeCondition = EnumTranslator::TransTimeType(aNewOrder.m_TimeInForceCode);

		lRet.ContingentCondition = EnumTranslator::TransTriggerType(aNewOrder.m_TriggerType);
		lRet.StopPrice =  AT::TransPriceToDouble(aNewOrder.m_TriggerPrice);

		lRet.VolumeTotalOriginal = aNewOrder.m_Vol;	///数量			
	}

	void CTP_TD::SetupAccountAndOthers( CThostFtdcInputOrderField &lRet, const AT::InputOrder &aNewOrder )
	{
		strcpy_s(lRet.BrokerID,11,m_BrokerID.c_str());
		strcpy_s(lRet.InvestorID, 13,m_UserID.c_str()); 	
		strcpy_s(lRet.InstrumentID, 31,aNewOrder.InstrumentID);	
		std::string lnextOrderRef = std::to_string(++m_OrderRef);
		strcpy_s(lRet.OrderRef,sizeof(lRet.OrderRef),lnextOrderRef.c_str());

		lRet.CombHedgeFlag[0] = THOST_FTDC_HF_Speculation;	  //组合投机套保标志
		lRet.VolumeCondition = THOST_FTDC_VC_AV; //成交量类型:任何数量
		lRet.MinVolume = 1;	//最小成交量:1	
		lRet.ForceCloseReason = THOST_FTDC_FCC_NotForceClose;	//强平原因:非强平	
		lRet.IsAutoSuspend = 0;  //自动挂起标志:否	
		lRet.UserForceClose = 0;   //用户强评标志:
	}

	void CTP_TD::SetupDirectionAndOpenClose( CThostFtdcInputOrderField &lRet,const AT::InputOrder& aNewOrder )
	{
		lRet.Direction = EnumTranslator::TransBuySellType(aNewOrder.m_BuySellType);
		lRet.CombOffsetFlag[0] =  EnumTranslator::TransOpenCloseType(aNewOrder.m_OpenCloseType);
	}

}
