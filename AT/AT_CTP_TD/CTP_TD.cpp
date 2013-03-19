#include "CTP_TD.h"
#include <iostream>
#include<boost\lexical_cast.hpp>
#include "ITradeSpi.h"
#include <vector>
#include <boost\tokenizer.hpp>
#include <boost\foreach.hpp>
#include <sstream>



extern "C"  __declspec(dllexport) AT::IDriver_TD* CreateDriverInsance(const std::map<std::string,std::string>& aConfig,  AT::ITradeSpi* apTradeSpi)
{
	std::string lIndex = aConfig.at("Index");
	static std::map<std::string,AT::IDriver_TD*> s_InstanceMap;
	if(s_InstanceMap.find(lIndex) != s_InstanceMap.end())
	{
		 AT::IDriver_TD* lpRet =  s_InstanceMap[lIndex];
		 lpRet->AddSpi(apTradeSpi);
		 return lpRet;
	}

	 AT::IDriver_TD* lpDriverInstance = new CTP::CTP_TD();
	 lpDriverInstance->Init(aConfig, apTradeSpi);
	 s_InstanceMap[lIndex] = lpDriverInstance;
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
		for(auto apSpi:m_TradeSpiPointMap)
		{
			apSpi.first->OnRtnState(m_RuningState,"");
		}
	}

	void CTP_TD::Init( const std::map<std::string,std::string>& aConfigMap,AT::ITradeSpi* apTradeSpi )
	{
		m_ConfigMap = aConfigMap;
		m_pDataCache.reset(new DataCache_CTP_TD(aConfigMap.at("Index")));
		m_TradeSpiPointMap.insert(std::make_pair(apTradeSpi,apTradeSpi));

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

	void CTP_TD::AddSpi( AT::ITradeSpi* apTradeSpi )
	{
		m_TradeSpiPointMap.insert(std::make_pair(apTradeSpi,apTradeSpi));
	}

	void CTP_TD::RemoveSpi( AT::ITradeSpi* apTradeSpi )
	{
		m_TradeSpiPointMap.erase(apTradeSpi);
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
		std::vector<std::string> lVarVec ;
		boost::tokenizer<> tok(aNewOrder);
		for(std::string lVar:tok)
		{
			lVarVec.push_back(lVar);
		}

		InputOrderTypePtr lRetPtr(new CThostFtdcInputOrderField);
		CThostFtdcInputOrderField& lRet = *lRetPtr;
		memset(&lRet,0,sizeof(CThostFtdcInputOrderField));
		strcpy_s(lRet.BrokerID,11,m_BrokerID.c_str());
		strcpy_s(lRet.InvestorID, 13,m_UserID.c_str()); //投资者代码	
		strcpy_s(lRet.InstrumentID, 31,lVarVec[order_name].c_str()); //合约代码	
		std::string lnextOrderRef = boost::lexical_cast<std::string>(++m_MaxOrderRef);
		strcpy(lRet.OrderRef,lnextOrderRef.c_str() );  //报单引用
		lRet.LimitPrice = boost::lexical_cast<double>(lVarVec[order_price]);	//价格
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
		lRet.Direction =  (lVarVec[order_buysell] == "buy")? THOST_FTDC_D_Buy:THOST_FTDC_D_Sell;  //买卖方向	
		char lopencloseFlag;
		if(lVarVec[order_openclose] == "open")
		{
			lopencloseFlag = THOST_FTDC_OF_Open;
		}
		else if(lVarVec[order_openclose] == "close")
		{
			lopencloseFlag = THOST_FTDC_OF_Close;
		}
		else if(lVarVec[order_openclose] == "closeT")
		{
			lopencloseFlag = THOST_FTDC_OF_CloseToday;
		}
		else if(lVarVec[order_openclose] == "closeY")
		{
			lopencloseFlag = THOST_FTDC_OF_CloseYesterday;
		}
		lRet.CombOffsetFlag[0] =  lopencloseFlag; //组合开平标志:开仓
		lRet.CombHedgeFlag[0] = THOST_FTDC_HF_Speculation;	  //组合投机套保标志	
		lRet.VolumeTotalOriginal = boost::lexical_cast<int>(lVarVec[order_val]);	///数量		
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
	{
		boost::shared_ptr<CThostFtdcOrderField> lpOrder (new CThostFtdcOrderField);
		memcpy(lpOrder.get(),pOrder,sizeof(CThostFtdcOrderField));
		m_pDataCache->UpdataOrder(lpOrder);
		for(std::pair<AT::ITradeSpi*,AT::ITradeSpi*> apSpi:m_TradeSpiPointMap)
		{
			apSpi.first->OnRtnOrder(BuildRtnOrderStr(lpOrder));
		}
	}

	void CTP_TD::OnRtnTrade( CThostFtdcTradeField *pTrade )
	{
		boost::shared_ptr<CThostFtdcTradeField> lpTrade (new CThostFtdcTradeField);
		memcpy(lpTrade.get(),pTrade,sizeof(CThostFtdcTradeField));
		m_pDataCache->UpdataTrade(lpTrade);
		for(std::pair<AT::ITradeSpi*,AT::ITradeSpi*> apSpi:m_TradeSpiPointMap)
		{
			apSpi.first->OnRtnTrade(BuildRtnTradeStr(lpTrade));
			apSpi.first->OnRtnState(Position_Change,m_pDataCache->GeneratorPositionString());
		}
	}

	std::string CTP_TD::BuildRtnTradeStr( boost::shared_ptr<CThostFtdcTradeField> apTrade )
	{
		std::string lExchangOrderID = GenerateExchangeOrderID(apTrade);
		std::string lThostOrderID = m_pDataCache->GetThostOrderIDByExchangeOrderID(lExchangOrderID);
		std::stringstream lOutbuf;
		lOutbuf << "RtnTrade" <<'\n'
			<<"ThostOrderID = " <<lThostOrderID <<'\n'
			<<"TradeID = "<<apTrade->TradeID <<'\n'
			<<"InstrumentID = " << apTrade->InstrumentID << '\n'
			<<"TradeVol = "<<apTrade->Volume<<'\n'
			<<"Price = "<<apTrade->Price<<'\n'
			<<"Time = "<<apTrade->TradeTime<<'\n';
		std::string lRet(lOutbuf.str());
		return lRet;
	}

	std::string CTP_TD::BuildRtnOrderStr( boost::shared_ptr<CThostFtdcOrderField> apOrder )
	{
		std::string lExchangOrderID = GenerateExchangeOrderID(apOrder);
		std::string lThostOrderID = m_pDataCache->GetThostOrderIDByExchangeOrderID(lExchangOrderID);
		std::stringstream lOutbuf;
		lOutbuf << "RtnOrder" <<'\n'
			<<"ThostOrderID = " <<lThostOrderID <<'\n'
			<<"ExchangOrderID = "<<lExchangOrderID <<'\n'
			<<"InstrumentID = " << apOrder->InstrumentID << '\n'
			<<"Vol_Orgi = "<<apOrder->VolumeTotalOriginal<<'\n'
			<<"Vol_Left = "<<apOrder->VolumeTotal<<'\n'
			<<"Vol_Today_Trade = "<<apOrder->VolumeTraded<<'\n'
			<<"Price = "<<apOrder->LimitPrice<<'\n'
			<<"BuySell = "<<apOrder->Direction<<'\n'
			<<"OpenClose = "<<apOrder->CombOffsetFlag[0]<<'\n'
			<<"OrderType = " <<apOrder->OrderType<<'\n'
			<<"TimeCodition = "<<apOrder->TimeCondition<<'\n'
			<<"OrderStatus = "<<apOrder->OrderStatus<<'\n'
			<<"OrderSubmitStatus = "<<apOrder->OrderSubmitStatus<<'\n'
			<<"Time = "<<apOrder->UpdateTime<<'\n'
			<<"Msg = "<<apOrder->StatusMsg<<'\n';
		std::string lRet(lOutbuf.str());
		return lRet;
	}

	void CTP_TD::OnRspOrderAction( CThostFtdcInputOrderActionField *pInputOrderAction, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast )
	{
		if(IsErrorRspInfo(pRspInfo))
		{
			boost::shared_ptr<CThostFtdcInputOrderActionField> lpRet(new CThostFtdcInputOrderActionField );
			memcpy(lpRet.get(),pInputOrderAction,sizeof(CThostFtdcInputOrderActionField));
			std::string lThostOrderID = GenerateThostOrderID(lpRet);
			std::stringstream lbuf;
			lbuf<<"Cancel Order Failed ThostOrderID "<< lThostOrderID;
			for(std::pair<AT::ITradeSpi*,AT::ITradeSpi*> apSpi:m_TradeSpiPointMap)
			{
				apSpi.first->OnRtnState(Cancel_Failed,lbuf.str());
			}
		}
	}

	void CTP_TD::OnRspOrderInsert( CThostFtdcInputOrderField *pInputOrder, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast )
	{
		if(IsErrorRspInfo(pRspInfo))
		{
			boost::shared_ptr<CThostFtdcInputOrderField> lpRet(new CThostFtdcInputOrderField );
			memcpy(lpRet.get(),pInputOrder,sizeof(CThostFtdcInputOrderField));
			std::string lThostOrderID = GenerateThostOrderID(lpRet,m_FrontID,m_SessionID);
			std::stringstream lbuf;
			lbuf<<"Create Order Failed ThostOrderID "<< lThostOrderID;
			for(auto apSpi : m_TradeSpiPointMap)
			{
				apSpi.first->OnRtnState(CreateOrder_Failed,lbuf.str());
			}
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
			for(std::pair<AT::ITradeSpi*,AT::ITradeSpi*> apSpi:m_TradeSpiPointMap)
			{
				apSpi.first->OnRtnState(QryPosition_Failed,"QryInvestorPosition  Failed");
			}
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
			for(std::pair<AT::ITradeSpi*,AT::ITradeSpi*> apSpi:m_TradeSpiPointMap)
			{
				apSpi.first->OnRtnPosition(lPosRspStr);
			}
			UpdateAccout();
		}
	}

	void CTP_TD::OnRspQryTradingAccount( CThostFtdcTradingAccountField *pTradingAccount, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast )
	{
		if(IsErrorRspInfo(pRspInfo))
		{
			for(std::pair<AT::ITradeSpi*,AT::ITradeSpi*> apSpi:m_TradeSpiPointMap)
			{
				apSpi.first->OnRtnState(QryAccout_Failed , "Qry Accout  Failed");
			}
		}
		boost::shared_ptr<CThostFtdcTradingAccountField> lpTradingAccout(new CThostFtdcTradingAccountField);
		memcpy(lpTradingAccout.get(),pTradingAccount,sizeof(CThostFtdcTradingAccountField));
		for(std::pair<AT::ITradeSpi*,AT::ITradeSpi*> apSpi:m_TradeSpiPointMap)
		{
			apSpi.first->OnRtnState(QryAccout_Succeed , BuildRtnAccoutStr(lpTradingAccout) );
		}
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
