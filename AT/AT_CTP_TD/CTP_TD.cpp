#include "CTP_TD.h"
#include "ITradeSpi.h"

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

		m_BrokerID  = lpt.get<std::string>("TDConfig.BrokeID");
		m_UserID  = lpt.get<std::string>("TDConfig.UserID");
		m_Password  = lpt.get<std::string>("TDConfig.Password");
		m_FrontAddress  = lpt.get<std::string>("TDConfig.Front");
		m_CTP_WorkFlowDir  = lpt.get<std::string>("TDConfig.WorkFlowDir");
	}


	void CTP_TD::OnFrontConnected()
	{
		CThostFtdcReqUserLoginField lLoginReq;
		memset(&lLoginReq,0,sizeof(CThostFtdcReqUserLoginField));
		strcpy_s(lLoginReq.BrokerID,11,m_BrokerID.c_str());
		strcpy_s(lLoginReq.UserID,15,m_UserID.c_str());
		strcpy_s(lLoginReq.Password,41,m_Password.c_str());

		int ret = m_pTraderAPI->ReqUserLogin(&lLoginReq,++m_RequestID);
		if(ret!= 0) std::cerr<<"ReqUserLogin Send Failed"<<std::endl;

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
			if(ret!=0) std::cerr<<"ReqQrySettlementInfoConfirm Send Failed"<<std::endl;
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
				if(ret!=0) std::cerr<<"ReqQryExchange Send Failed"<<std::endl;
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
		std::cout<<"Receive Order \n\n\n"<<aNewOrder.ToString();
		InputOrderTypePtr lExchangeOrder = BuildExchangeOrder(aNewOrder);
		int ret = m_pTraderAPI->ReqOrderInsert(lExchangeOrder.get(),++m_RequestID);
		if(ret!=0)
		{
			std::cerr<<"CreateOrder  Failed"<<std::endl;
			std::shared_ptr<AT::OrderUpdate> lRejectOrder (new AT::OrderUpdate);
			AT::CopyOrderFields(*lRejectOrder,aNewOrder);
			lRejectOrder->m_UpdateTime = AT::AT_Local_Time();
			lRejectOrder->m_OrderStatus = AT::OrderStatusType::RejectOrder;
			lRejectOrder->m_TradedVol = 0;
			lRejectOrder->m_LiveVol = 0;
			m_IO_Service.post(std::bind(&CTP_TD::SendOrderUpdate,this,lRejectOrder));
		}
		else
		{
			std::cout<<"Send Create Order Succeed ";
		}
	}


	//void CTP_TD::DeleteOrder( const std::string& aClientOrderID )
	//{
	//	boost::shared_ptr<CThostFtdcOrderField> lExchangOrderPtr = m_pDataCache->FindOrderByThostID(aClientOrderID);

	//	if(!lExchangOrderPtr)
	//	{
	//		//LogError
	//		return;
	//	}
	//	int lFrontID ;
	//	int lSessionID;
	//	std::string lOrderRef = ResolveThostOrderID(aClientOrderID,lSessionID,lFrontID);

	//	CThostFtdcInputOrderActionField  lOrderAction ;
	//	memset(&lOrderAction,0,sizeof(lOrderAction));
	//	lOrderAction.ActionFlag =THOST_FTDC_AF_Delete;
	//	strcpy_s(lOrderAction.BrokerID ,sizeof(lOrderAction.BrokerID), lExchangOrderPtr->BrokerID);
	//	lOrderAction.FrontID =lFrontID;
	//	lOrderAction.SessionID = lSessionID;
	//	strcpy_s(lOrderAction.InstrumentID ,sizeof(lOrderAction.InstrumentID) ,lExchangOrderPtr->InstrumentID);
	//	strcpy_s(lOrderAction.InvestorID ,sizeof(lOrderAction.InvestorID),lExchangOrderPtr->InvestorID);
	//	lOrderAction.LimitPrice = lExchangOrderPtr->LimitPrice;
	//	strcpy_s(lOrderAction.OrderRef ,sizeof(lOrderAction.OrderRef) ,lExchangOrderPtr->OrderRef);
	//	strcpy_s(lOrderAction.UserID ,sizeof(lOrderAction.UserID) ,lExchangOrderPtr->UserID);
	//	int ret = m_pTraderAPI->ReqOrderAction(&lOrderAction,++m_RequestID);
	//	if(ret != 0)  std::cerr<<"DeleteOrder Send Failed"<<std::endl;
	//}




	void CTP_TD::OnFrontDisconnected( int nReason )
	{
;
	}



	CTP::InputOrderTypePtr CTP_TD::BuildExchangeOrder( const AT::InputOrder& aNewOrder )
	{

		InputOrderTypePtr lRetPtr(new CThostFtdcInputOrderField);
		memset(lRetPtr.get(),0,sizeof(CThostFtdcInputOrderField));
		CThostFtdcInputOrderField& lRet = *lRetPtr;
		strcpy_s(lRet.BrokerID,11,m_BrokerID.c_str());
		strcpy_s(lRet.InvestorID, 13,m_UserID.c_str()); 	
		strcpy_s(lRet.InstrumentID, 31,aNewOrder.InstrumentID);	
		std::string lnextOrderRef = std::to_string(++m_OrderRef);
		strcpy_s(lRet.OrderRef,sizeof(lRet.OrderRef),lnextOrderRef.c_str());

		lRet.LimitPrice = AT::TransPriceToDouble(aNewOrder.m_Price);	//价格
		if(abs(lRet.LimitPrice)<0.01)
		{
			lRet.OrderPriceType = THOST_FTDC_OPT_AnyPrice;//价格类型=市价
			lRet.TimeCondition = THOST_FTDC_TC_IOC;//有效期类型:立即完成，否则撤销
		}
		else
		{
			lRet.OrderPriceType = THOST_FTDC_OPT_LimitPrice;//价格类型=限价	
			lRet.TimeCondition = THOST_FTDC_TC_GFD;  //有效期类型:当日有效
		}
		lRet.Direction =  (aNewOrder.m_BuySellType ==AT::BuySellType::BuyOrder )? THOST_FTDC_D_Buy:THOST_FTDC_D_Sell;  //买卖方向	
		char lopencloseFlag;

		switch (aNewOrder.m_OpenCloseType)
		{
		case AT::OpenCloseType::CloseOrder:
			lopencloseFlag = THOST_FTDC_OF_Close;
			//lopencloseFlag = THOST_FTDC_OF_CloseYesterday;
			break;
		case AT::OpenCloseType::CloseTodayOrder :
			lopencloseFlag = THOST_FTDC_OF_CloseToday;
			break;
		case AT::OpenCloseType::OpenOrder:
				lopencloseFlag = THOST_FTDC_OF_Open;
			break;
		}

		lRet.CombOffsetFlag[0] =  lopencloseFlag; //组合开平标志:开仓
		lRet.CombHedgeFlag[0] = THOST_FTDC_HF_Speculation;	  //组合投机套保标志	
		lRet.VolumeTotalOriginal = aNewOrder.m_Vol;	///数量		
		lRet.VolumeCondition = THOST_FTDC_VC_AV; //成交量类型:任何数量
		lRet.MinVolume = 1;	//最小成交量:1	
		lRet.ContingentCondition = THOST_FTDC_CC_Immediately;  //触发条件:立即
		lRet.ForceCloseReason = THOST_FTDC_FCC_NotForceClose;	//强平原因:非强平	
		lRet.IsAutoSuspend = 0;  //自动挂起标志:否	
		lRet.UserForceClose = 0;   //用户强评标志:
		return lRetPtr;
	}


	void CTP_TD::OnRtnOrder( CThostFtdcOrderField *pOrder )
	try
	{
		boost::shared_ptr<CThostFtdcOrderField> lpOrder (new CThostFtdcOrderField);
		memcpy(lpOrder.get(),pOrder,sizeof(CThostFtdcOrderField));
		//m_pDataCache->UpdataOrder(lpOrder);
		//m_pTradeSpi->OnRtnOrder(BuildRtnOrderStr(lpOrder));
	}
	catch(std::exception& ex)
	{
		std::cerr<<ex.what();
	}

	void CTP_TD::OnRtnTrade( CThostFtdcTradeField *pTrade )
	try
	{
		boost::shared_ptr<CThostFtdcTradeField> lpTrade (new CThostFtdcTradeField);
		memcpy(lpTrade.get(),pTrade,sizeof(CThostFtdcTradeField));
	//	m_pDataCache->UpdataTrade(lpTrade);
	//	m_pTradeSpi->OnRtnTrade(BuildRtnTradeStr(lpTrade));
		//m_pTradeSpi->OnRtnState(Position_Change,m_pDataCache->GeneratorPositionString());
	}
	catch (std::exception& ex)
	{
		std::cerr<<ex.what();
	};

	//std::string CTP_TD::BuildRtnTradeStr( std::shared_ptr<CThostFtdcTradeField> apTrade )
	//{
	//
	//
	//	std::string lExchangOrderID = GenerateExchangeOrderID(apTrade);
	//	std::string lThostOrderID = m_pDataCache->GetThostOrderIDByExchangeOrderID(lExchangOrderID);

	//	using boost::property_tree::ptree;
	//	ptree pt;
	//	pt.put("head.type","TradeUpdate");
	//	pt.put("head.version",0.1f);
	//	pt.put("Order.AccountID",apTrade->UserID);
	//	pt.put("Trade.ThostOrderID" , lThostOrderID );
	//	pt.put("Trade.TradeID" , apTrade->TradeID  );
	//	pt.put("Trade.InstrumentID" ,apTrade->InstrumentID  );
	//	pt.put("Trade.TradeVol" , apTrade->Volume );
	//	pt.put("Trade.Price" , apTrade->Price );
	//	pt.put("Trade.Time" , apTrade->TradeTime);

	//	
	//	std::stringstream lStringStream;
	//	write_xml(lStringStream,pt);
	//	return lStringStream.str();
	//}

	//std::string CTP_TD::BuildRtnOrderStr( boost::shared_ptr<CThostFtdcOrderField> apOrder )
	//{
	//	std::string lExchangOrderID = GenerateExchangeOrderID(apOrder);
	//	std::string lThostOrderID = m_pDataCache->GetThostOrderIDByExchangeOrderID(lExchangOrderID);
	//	
	//	using boost::property_tree::ptree;
	//	ptree pt;
	//	pt.put("head.type","OrderUpdate");
	//	pt.put("head.version",0.1f);
	//	pt.put("Order.AccountID",apOrder->UserID);
	//	pt.put("Order.ThostOrderID" , lThostOrderID );
	//	pt.put("Order.ExchangOrderID" , lExchangOrderID  );
	//	pt.put("Order.InstrumentID" ,apOrder->InstrumentID );
	//	pt.put("Order.TotalVol" ,apOrder->VolumeTotalOriginal);
	//	pt.put("Order.TradeVol" , apOrder->VolumeTraded);
	//	pt.put("Order.Price" ,apOrder->LimitPrice );
	//	pt.put("Order.UpdateTime" ,apOrder->UpdateTime);
	//	pt.put("BuySell = ",apOrder->Direction);
	//	pt.put("Order.OpenClose" ,apOrder->CombOffsetFlag[0] );
	//	pt.put("Order.OrderType" ,apOrder->OrderType);
	//	pt.put("Order.TimeCodition" , apOrder->TimeCondition);
	//	pt.put("Order.OrderStatus" ,apOrder->OrderStatus );
	//	pt.put("Order.OrderSubmitStatus" ,apOrder->OrderSubmitStatus);
	//	pt.put("StatusMsg",apOrder->StatusMsg);	
	//	
	//	std::stringstream lStringStream;
	//	write_xml(lStringStream,pt);
	//	return lStringStream.str();
	//	
	//}

	//void CTP_TD::OnRspOrderAction( CThostFtdcInputOrderActionField *pInputOrderAction, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast )
	//{
	//	if(IsErrorRspInfo(pRspInfo))
	//	{
	//		//todo cancel reject
	//		boost::shared_ptr<CThostFtdcInputOrderActionField> lpRet(new CThostFtdcInputOrderActionField );
	//		memcpy(lpRet.get(),pInputOrderAction,sizeof(CThostFtdcInputOrderActionField));
	//		std::string lThostOrderID = GenerateThostOrderID(lpRet);
	//		std::stringstream lbuf;
	//		lbuf<<"Cancel Order Failed ThostOrderID =  "<< lThostOrderID << "UserID "<<m_UserID;
	//		m_pTradeSpi->OnRtnState(Cancel_Failed,lbuf.str());
	//	}
	//}

	void CTP_TD::OnRspOrderInsert( CThostFtdcInputOrderField *pInputOrder, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast )
	{
		if(IsErrorRspInfo(pRspInfo))
		{
			//todo check this message is in current session
			//how to handle it is a problem 
			//because it not carray frontID and SessionID
			boost::shared_ptr<CThostFtdcInputOrderField> lpRet(new CThostFtdcInputOrderField );
			memcpy(lpRet.get(),pInputOrder,sizeof(CThostFtdcInputOrderField));
			std::string lThostOrderID = GenerateThostOrderID(lpRet,m_FrontID,m_SessionID);
			
			using boost::property_tree::ptree;
			ptree pt;
			pt.put("head.type","OrderUpdate");
			pt.put("head.version",0.1f);
			pt.put("Order.AccountID",m_UserID);
			pt.put("Order.ThostOrderID" , lThostOrderID );
			pt.put("Order.OrderStatus" ,THOST_FTDC_OST_Canceled);
			std::stringstream lStringStream;
			write_xml(lStringStream,pt);
			m_pTradeSpi->OnRtnOrder(lStringStream.str());
		}
	}

	//void CTP_TD::UpdateAccout()
	//{
	//	CThostFtdcQryTradingAccountField req;
	//	memset(&req, 0, sizeof(req));
	//	strcpy_s(req.BrokerID, sizeof(req.BrokerID),m_BrokerID.c_str());
	//	strcpy_s(req.InvestorID,sizeof(req.InvestorID), m_UserID.c_str());
	//	int ret = m_pTraderAPI->ReqQryTradingAccount(&req, ++m_RequestID);
	//	if(ret != 0)  std::cerr<<"QryTradingAccount Send Failed"<<std::endl;
	//}

	//void CTP_TD::OnRspQryInvestorPosition( CThostFtdcInvestorPositionField *pInvestorPosition, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast )
	//{
	//	if(IsErrorRspInfo(pRspInfo))
	//	{
	//		//todo add User Tag
	//		m_pTradeSpi->OnRtnState(QryPosition_Failed,"QryInvestorPosition  Failed");
	//		m_IsInQryPosition = false; 
	//	}
	//	if(NULL!=pInvestorPosition)
	//	{
	//		boost::shared_ptr<CThostFtdcInvestorPositionField>  lpPos(new CThostFtdcInvestorPositionField);
	//		memcpy(lpPos.get(),pInvestorPosition,sizeof(CThostFtdcInvestorPositionField));
	//		m_pDataCache->UpdatePosition(lpPos);
	//	}


	//	if(bIsLast)
	//	{
	//		m_IsInQryPosition = false; 
	//		std::string lPosRspStr = m_pDataCache->GeneratorPositionString();
	//		m_pTradeSpi->OnRtnPosition(lPosRspStr);
	//		UpdateAccout();
	//	}
	//}

	//void CTP_TD::OnRspQryTradingAccount( CThostFtdcTradingAccountField *pTradingAccount, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast )
	//{
	//	if(IsErrorRspInfo(pRspInfo))
	//	{
	//		m_pTradeSpi->OnRtnState(QryAccout_Failed , "Qry Accout  Failed");
	//	}
	//	boost::shared_ptr<CThostFtdcTradingAccountField> lpTradingAccout(new CThostFtdcTradingAccountField);
	//	memcpy(lpTradingAccout.get(),pTradingAccount,sizeof(CThostFtdcTradingAccountField));
	//	m_pTradeSpi->OnRtnState(QryAccout_Succeed , BuildRtnAccoutStr(lpTradingAccout) );
	//}

	//std::string CTP_TD::BuildRtnAccoutStr( boost::shared_ptr<CThostFtdcTradingAccountField> apAccout )
	//{
	//	std::stringstream lbuf;
	//	lbuf<< "AccountID = "<<apAccout->AccountID<<'\n'
	//		<<"FrozenMargin = "<<apAccout->FrozenMargin<<'\n'
	//		<<"FrozenCash = "<<apAccout->FrozenCash<<'\n'
	//		<<"CurrMargin = "<<apAccout->CurrMargin<<'\n'
	//		<<"CloseProfit = "<<apAccout->CloseProfit<<'\n'
	//		<<"PositionProfit = "<<apAccout->PositionProfit<<'\n'
	//		<<"Balance = "<<apAccout->Balance<<'\n'
	//		<<"Available = "<<apAccout->Available<<'\n';
	//	std::string lRet =lbuf.str();
	//	return lRet;
	//}

	void CTP_TD::SendOrderUpdate( std::shared_ptr<AT::OrderUpdate> apOrderUpdate )
	{
		std::cout<< apOrderUpdate->ToString();
		m_pTradeSpi->OnRtnOrder(*apOrderUpdate);
	}

	void CTP_TD::SendTradeUpdate( std::shared_ptr<AT::TradeUpdate > apTradeUpdate )
	{
		std::cout<< apTradeUpdate->ToString();
		m_pTradeSpi->OnRtnTrade(*apTradeUpdate);
	}

	void CTP_TD::DeleteOrder( const AT::CancelOrder& aDelOrderID )
	{

	}

	void CTP_TD::ModifyOrder( const AT::ModifyOrder& aRequest )
	{

	}

	void CTP_TD::Stop()
	{

	}




}
