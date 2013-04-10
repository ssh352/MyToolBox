#include "CTP_TD.h"
#include <iostream>
#include<boost\lexical_cast.hpp>
#include "ITradeSpi.h"
#include <vector>
#include <boost\tokenizer.hpp>
#include  "MyForeach.h"
#include <sstream>
#include <boost\thread.hpp>
#include <boost\date_time.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>

extern "C"  __declspec(dllexport) AT::IDriver_TD* CreateDriverInsance(const std::map<std::string,std::string>& aConfig,  AT::ITradeSpi* apTradeSpi)
{
	 AT::IDriver_TD* lpDriverInstance = new CTP::CTP_TD();
	 lpDriverInstance->Init(aConfig, apTradeSpi);
	 return lpDriverInstance;
}

char g_Address[512] ;
namespace CTP
{
	CTP_TD::CTP_TD(void)
		: m_RequestID(0)
		, m_RuningState(UnInit)
		, m_IsInQryPosition(false)
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
	}

	void CTP_TD::NotifyState()
	{
		m_pTradeSpi->OnRtnState(m_RuningState,m_UserID);
	}

	void CTP_TD::Init( const std::map<std::string,std::string>& aConfigMap,AT::ITradeSpi* apTradeSpi )
	{
		m_ConfigMap = aConfigMap;
		m_pDataCache.reset(new DataCache_CTP_TD(aConfigMap.at("StoreDir")));
		m_pTradeSpi = apTradeSpi;


		m_BrokerID = m_ConfigMap["BrokerID"];
		m_UserID = m_ConfigMap["UserID"];
		m_Password = m_ConfigMap["Password"];
		m_pTraderAPI = CThostFtdcTraderApi::CreateFtdcTraderApi();
		m_pTraderAPI->RegisterSpi(this);		
		m_pTraderAPI->SubscribePublicTopic(THOST_TERT_RESTART);					
		m_pTraderAPI->SubscribePrivateTopic(THOST_TERT_RESTART);

		memset(g_Address,0,512);
		strcpy_s(g_Address,512,m_ConfigMap["Front"].c_str());

		std::cerr<<"BrokerID "<< m_BrokerID<<'\n'
			<<"UserID "<< m_UserID<<'\n'
			<<"Password "<< m_Password<<'\n'
			<<"Front "<< g_Address<<'\n'<<std::endl;
		m_pTraderAPI->RegisterFront(g_Address);
		m_pTraderAPI->Init();
		m_RuningState = Connecting;
		NotifyState();
	}

	void CTP_TD::OnFrontConnected()
	{
		CThostFtdcReqUserLoginField lLoginReq;
		memset(&lLoginReq,0,sizeof(CThostFtdcReqUserLoginField));
		strcpy_s(lLoginReq.BrokerID,11,m_BrokerID.c_str());
		strcpy_s(lLoginReq.UserID,15,m_UserID.c_str());
		strcpy_s(lLoginReq.Password,41,m_Password.c_str());

		int ret = m_pTraderAPI->ReqUserLogin(&lLoginReq,++m_RequestID);
		if(ret!=0) std::cerr<<"ReqUserLogin Send Failed"<<std::endl;

		m_RuningState = Logining;
		 NotifyState();
	}

	void CTP_TD::OnRspUserLogin( CThostFtdcRspUserLoginField *apRspUserLogin, CThostFtdcRspInfoField *apRspInfo, int anRequestID, bool abIsLast )
	{
		if(IsErrorRspInfo(apRspInfo))
		{
			m_RuningState = LoginFail;
			NotifyState();
			return;
		}
		else
		{
			m_SessionID = apRspUserLogin->SessionID;
			m_FrontID = apRspUserLogin->FrontID;
			m_MaxOrderRef = boost::lexical_cast<unsigned int>(apRspUserLogin->MaxOrderRef);
			m_RuningState =ConfirmingSettlement;
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
			m_RuningState = LoginFail;
			NotifyState();
			return;
		}
		else
		{
			QueryPosition("");
			if(apSettlementInfoConfirm!= NULL)
			{
				m_RuningState = Ready;
				NotifyState();
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
			m_RuningState = Ready;
			NotifyState();
		}
	}

	std::string CTP_TD::CreateOrder( const std::string& aNewOrder )
	{
		InputOrderTypePtr lExchangeOrder = BuildExchangeOrder(aNewOrder);
		int ret = m_pTraderAPI->ReqOrderInsert(lExchangeOrder.get(),++m_RequestID);

		CThostFtdcInputOrderFieldTraits::SetFrontID(m_FrontID);
		CThostFtdcInputOrderFieldTraits::SetSessionID(m_SessionID);
		m_pDataCache->UpdataInputOrder(lExchangeOrder);
		if(ret!=0)
		{
			 std::cerr<<"CreateOrder Send Failed"<<std::endl;
			 return std::string();
		}
		else
		{
			std::string lThostOrderID = GenerateThostOrderID(lExchangeOrder,m_FrontID,m_SessionID);
			return lThostOrderID;
		}
	}

	void CTP_TD::DeleteOrder( const std::string& aClientOrderID )
	{
		boost::shared_ptr<CThostFtdcOrderField> lExchangOrderPtr = m_pDataCache->FindOrderByThostID(aClientOrderID);

		if(!lExchangOrderPtr)
		{
			//LogError
			return;
		}
		int lFrontID ;
		int lSessionID;
		std::string lOrderRef = ResolveThostOrderID(aClientOrderID,lSessionID,lFrontID);

		CThostFtdcInputOrderActionField  lOrderAction ;
		memset(&lOrderAction,0,sizeof(lOrderAction));
		lOrderAction.ActionFlag =THOST_FTDC_AF_Delete;
		strcpy(lOrderAction.BrokerID, lExchangOrderPtr->BrokerID);
		lOrderAction.FrontID =lFrontID;
		lOrderAction.SessionID = lSessionID;
		strcpy(lOrderAction.InstrumentID,lExchangOrderPtr->InstrumentID);
		strcpy(lOrderAction.InvestorID ,lExchangOrderPtr->InvestorID);
		lOrderAction.LimitPrice = lExchangOrderPtr->LimitPrice;
		strcpy(lOrderAction.OrderRef,lExchangOrderPtr->OrderRef);
		strcpy(lOrderAction.UserID,lExchangOrderPtr->UserID);
		int ret = m_pTraderAPI->ReqOrderAction(&lOrderAction,++m_RequestID);
		if(ret != 0)  std::cerr<<"DeleteOrder Send Failed"<<std::endl;
	}

	void CTP_TD::ModifyOrder( const std::string& aRequest )
	{

	}

	void CTP_TD::QueryPosition( const std::string& aRequest )
	{
		//no need for anyinfo just queryposition
		{
			CThostFtdcQryInvestorPositionField req;
			memset(&req, 0, sizeof(req));
			strcpy(req.BrokerID, m_BrokerID.c_str());
			strcpy(req.InvestorID, m_UserID.c_str());
			boost::this_thread::sleep(boost::posix_time::seconds(2));
			int ret = m_pTraderAPI->ReqQryInvestorPosition(&req, ++m_RequestID);
			if(ret != 0)  std::cerr<<"QryInvestorPosition Send Failed"<<std::endl;
			m_IsInQryPosition = true;
			m_pDataCache->ClearPosition();
		}
	}

	void CTP_TD::OnFrontDisconnected( int nReason )
	{
		m_RuningState = Disconnect;
		NotifyState();
	}

	InputOrderTypePtr CTP_TD::BuildExchangeOrder( const std::string& aNewOrder )
	{
		std::stringstream lbuf(aNewOrder);
		using boost::property_tree::ptree;
		ptree pt;
		read_xml(lbuf,pt);
		
		//todo check head version

		
		std::string lInstrument =pt.get<std::string>("Order.ID");
		double lPrice = pt.get<double> ("Order.Price");
		std::string lBuySellCode = pt.get<std::string> ("Order.BuyCode" );
		std::string lOpenCloseCode = pt.get<std::string>("Order.OpenCode");
		int lVol = pt.get<int>("Order.Vol");
		

		InputOrderTypePtr lRetPtr(new CThostFtdcInputOrderField);
		CThostFtdcInputOrderField& lRet = *lRetPtr;
		memset(&lRet,0,sizeof(CThostFtdcInputOrderField));
		strcpy_s(lRet.BrokerID,11,m_BrokerID.c_str());
		strcpy_s(lRet.InvestorID, 13,m_UserID.c_str()); //投资者代码	
		strcpy_s(lRet.InstrumentID, 31,lInstrument.c_str()); //合约代码	
		std::string lnextOrderRef = boost::lexical_cast<std::string>(++m_MaxOrderRef);
		strcpy(lRet.OrderRef,lnextOrderRef.c_str() );  //报单引用
		lRet.LimitPrice = lPrice;	//价格
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
		lRet.Direction =  (lBuySellCode== "Buy")? THOST_FTDC_D_Buy:THOST_FTDC_D_Sell;  //买卖方向	
		char lopencloseFlag;
		if(lOpenCloseCode== "Open")
		{
			lopencloseFlag = THOST_FTDC_OF_Open;
		}
		else if(lOpenCloseCode == "Close")
		{
			lopencloseFlag = THOST_FTDC_OF_Close;
		}
		else if(lOpenCloseCode == "CloseT")
		{
			lopencloseFlag = THOST_FTDC_OF_CloseToday;
		}
		else if(lOpenCloseCode == "CloseY")
		{
			lopencloseFlag = THOST_FTDC_OF_CloseYesterday;
		}
		lRet.CombOffsetFlag[0] =  lopencloseFlag; //组合开平标志:开仓
		lRet.CombHedgeFlag[0] = THOST_FTDC_HF_Speculation;	  //组合投机套保标志	
		lRet.VolumeTotalOriginal = lVol;	///数量		
		lRet.VolumeCondition = THOST_FTDC_VC_AV; //成交量类型:任何数量
		lRet.MinVolume = 1;	//最小成交量:1	
		lRet.ContingentCondition = THOST_FTDC_CC_Immediately;  //触发条件:立即
		//TThostFtdcPriceType	StopPrice;  //止损价
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
		m_pDataCache->UpdataOrder(lpOrder);
		m_pTradeSpi->OnRtnOrder(BuildRtnOrderStr(lpOrder));
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
		m_pTradeSpi->OnRtnTrade(BuildRtnTradeStr(lpTrade));
		//m_pTradeSpi->OnRtnState(Position_Change,m_pDataCache->GeneratorPositionString());
	}
	catch (std::exception& ex)
	{
		std::cerr<<ex.what();
	};

	std::string CTP_TD::BuildRtnTradeStr( boost::shared_ptr<CThostFtdcTradeField> apTrade )
	{
	
	
		std::string lExchangOrderID = GenerateExchangeOrderID(apTrade);
		std::string lThostOrderID = m_pDataCache->GetThostOrderIDByExchangeOrderID(lExchangOrderID);

		using boost::property_tree::ptree;
		ptree pt;
		pt.put("head.type","TradeUpdate");
		pt.put("head.version",0.1f);
		pt.put("Order.AccountID",apTrade->UserID);
		pt.put("Trade.ThostOrderID" , lThostOrderID );
		pt.put("Trade.TradeID" , apTrade->TradeID  );
		pt.put("Trade.InstrumentID" ,apTrade->InstrumentID  );
		pt.put("Trade.TradeVol" , apTrade->Volume );
		pt.put("Trade.Price" , apTrade->Price );
		pt.put("Trade.Time" , apTrade->TradeTime);

		
		std::stringstream lStringStream;
		write_xml(lStringStream,pt);
		return lStringStream.str();
	}

	std::string CTP_TD::BuildRtnOrderStr( boost::shared_ptr<CThostFtdcOrderField> apOrder )
	{
		std::string lExchangOrderID = GenerateExchangeOrderID(apOrder);
		std::string lThostOrderID = m_pDataCache->GetThostOrderIDByExchangeOrderID(lExchangOrderID);
		
		using boost::property_tree::ptree;
		ptree pt;
		pt.put("head.type","OrderUpdate");
		pt.put("head.version",0.1f);
		pt.put("Order.AccountID",apOrder->UserID);
		pt.put("Order.ThostOrderID" , lThostOrderID );
		pt.put("Order.ExchangOrderID" , lExchangOrderID  );
		pt.put("Order.InstrumentID" ,apOrder->InstrumentID );
		pt.put("Order.TotalVol" ,apOrder->VolumeTotalOriginal);
		pt.put("Order.TradeVol" , apOrder->VolumeTraded);
		pt.put("Order.Price" ,apOrder->LimitPrice );
		pt.put("Order.UpdateTime" ,apOrder->UpdateTime);
		pt.put("BuySell = ",apOrder->Direction);
		pt.put("Order.OpenClose" ,apOrder->CombOffsetFlag[0] );
		pt.put("Order.OrderType" ,apOrder->OrderType);
		pt.put("Order.TimeCodition" , apOrder->TimeCondition);
		pt.put("Order.OrderStatus" ,apOrder->OrderStatus );
		pt.put("Order.OrderSubmitStatus" ,apOrder->OrderSubmitStatus);
		pt.put("StatusMsg",apOrder->StatusMsg);	
		
		std::stringstream lStringStream;
		write_xml(lStringStream,pt);
		return lStringStream.str();
		
	}

	void CTP_TD::OnRspOrderAction( CThostFtdcInputOrderActionField *pInputOrderAction, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast )
	{
		if(IsErrorRspInfo(pRspInfo))
		{
			//todo cancel reject
			boost::shared_ptr<CThostFtdcInputOrderActionField> lpRet(new CThostFtdcInputOrderActionField );
			memcpy(lpRet.get(),pInputOrderAction,sizeof(CThostFtdcInputOrderActionField));
			std::string lThostOrderID = GenerateThostOrderID(lpRet);
			std::stringstream lbuf;
			lbuf<<"Cancel Order Failed ThostOrderID =  "<< lThostOrderID << "UserID "<<m_UserID;
			m_pTradeSpi->OnRtnState(Cancel_Failed,lbuf.str());
		}
	}

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

	void CTP_TD::UpdateAccout()
	{
		CThostFtdcQryTradingAccountField req;
		memset(&req, 0, sizeof(req));
		strcpy(req.BrokerID, m_BrokerID.c_str());
		strcpy(req.InvestorID, m_UserID.c_str());
		int ret = m_pTraderAPI->ReqQryTradingAccount(&req, ++m_RequestID);
		if(ret != 0)  std::cerr<<"QryTradingAccount Send Failed"<<std::endl;
	}

	void CTP_TD::OnRspQryInvestorPosition( CThostFtdcInvestorPositionField *pInvestorPosition, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast )
	{
		if(IsErrorRspInfo(pRspInfo))
		{
			//todo add User Tag
			m_pTradeSpi->OnRtnState(QryPosition_Failed,"QryInvestorPosition  Failed");
			m_IsInQryPosition = false; 
		}
		if(NULL!=pInvestorPosition)
		{
			boost::shared_ptr<CThostFtdcInvestorPositionField>  lpPos(new CThostFtdcInvestorPositionField);
			memcpy(lpPos.get(),pInvestorPosition,sizeof(CThostFtdcInvestorPositionField));
			m_pDataCache->UpdatePosition(lpPos);
		}


		if(bIsLast)
		{
			m_IsInQryPosition = false; 
			std::string lPosRspStr = m_pDataCache->GeneratorPositionString();
			m_pTradeSpi->OnRtnPosition(lPosRspStr);
			UpdateAccout();
		}
	}

	void CTP_TD::OnRspQryTradingAccount( CThostFtdcTradingAccountField *pTradingAccount, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast )
	{
		if(IsErrorRspInfo(pRspInfo))
		{
			m_pTradeSpi->OnRtnState(QryAccout_Failed , "Qry Accout  Failed");
		}
		boost::shared_ptr<CThostFtdcTradingAccountField> lpTradingAccout(new CThostFtdcTradingAccountField);
		memcpy(lpTradingAccout.get(),pTradingAccount,sizeof(CThostFtdcTradingAccountField));
		m_pTradeSpi->OnRtnState(QryAccout_Succeed , BuildRtnAccoutStr(lpTradingAccout) );
	}

	std::string CTP_TD::BuildRtnAccoutStr( boost::shared_ptr<CThostFtdcTradingAccountField> apAccout )
	{
		std::stringstream lbuf;
		lbuf<< "AccountID = "<<apAccout->AccountID<<'\n'
			<<"FrozenMargin = "<<apAccout->FrozenMargin<<'\n'
			<<"FrozenCash = "<<apAccout->FrozenCash<<'\n'
			<<"CurrMargin = "<<apAccout->CurrMargin<<'\n'
			<<"CloseProfit = "<<apAccout->CloseProfit<<'\n'
			<<"PositionProfit = "<<apAccout->PositionProfit<<'\n'
			<<"Balance = "<<apAccout->Balance<<'\n'
			<<"Available = "<<apAccout->Available<<'\n';
		std::string lRet =lbuf.str();
		return lRet;
	}
}
