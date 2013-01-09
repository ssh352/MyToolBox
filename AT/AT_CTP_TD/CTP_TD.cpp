#include "CTP_TD.h"
#include <iostream>
#include<boost\lexical_cast.hpp>
#include "ITradeSpi.h"
#include <vector>
#include <boost\tokenizer.hpp>
#include <boost\foreach.hpp>
#include <sstream>

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
		m_pTradeSpi->OnRtnState(m_RuningState,"");
	}

	void CTP_TD::Init( const std::map<std::string,std::string>& aConfigMap,AT::ITradeSpi* apTradeSpi )
	{
		m_ConfigMap = aConfigMap;
		m_pTradeSpi = apTradeSpi;

		m_BrokerID = m_ConfigMap["BrokerID"];
		m_UserID = m_ConfigMap["UserID"];
		m_Password = m_ConfigMap["Password"];
		m_pTraderAPI = CThostFtdcTraderApi::CreateFtdcTraderApi(".\\TDflow1\\");
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
		m_DataCache.UpdataInputOrder(lExchangeOrder);
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
		InputOrderTypePtr lExchangOrderPtr = m_DataCache. FindInputOrderByThostID(aClientOrderID);

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
			m_DataCache.ClearPosition();
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
		BOOST_FOREACH(std::string lVar,tok)
		{
			lVarVec.push_back(lVar);
		}

		InputOrderTypePtr lRetPtr(new CThostFtdcInputOrderField);
		CThostFtdcInputOrderField& lRet = *lRetPtr;
		memset(&lRet,0,sizeof(CThostFtdcInputOrderField));
		strcpy_s(lRet.BrokerID,11,m_BrokerID.c_str());
		strcpy_s(lRet.InvestorID, 13,m_UserID.c_str()); //Ͷ���ߴ���	
		strcpy_s(lRet.InstrumentID, 31,lVarVec[order_name].c_str()); //��Լ����	
		std::string lnextOrderRef = boost::lexical_cast<std::string>(++m_MaxOrderRef);
		strcpy(lRet.OrderRef,lnextOrderRef.c_str() );  //��������
		lRet.LimitPrice = boost::lexical_cast<double>(lVarVec[order_price]);	//�۸�
		if(abs(lRet.LimitPrice)<0.01)
		{
			lRet.OrderPriceType = THOST_FTDC_OPT_AnyPrice;//�۸�����=�м�
			lRet.TimeCondition = THOST_FTDC_TC_IOC;//��Ч������:������ɣ�������
		}
		else
		{
			lRet.OrderPriceType = THOST_FTDC_OPT_LimitPrice;//�۸�����=�޼�	
			lRet.TimeCondition = THOST_FTDC_TC_GFD;  //��Ч������:������Ч
		}
		lRet.Direction =  (lVarVec[order_buysell] == "buy")? THOST_FTDC_D_Buy:THOST_FTDC_D_Sell;  //��������	
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
		lRet.CombOffsetFlag[0] =  lopencloseFlag; //��Ͽ�ƽ��־:����
		lRet.CombHedgeFlag[0] = THOST_FTDC_HF_Speculation;	  //���Ͷ���ױ���־	
		lRet.VolumeTotalOriginal = boost::lexical_cast<int>(lVarVec[order_val]);	///����		
		lRet.VolumeCondition = THOST_FTDC_VC_AV; //�ɽ�������:�κ�����
		lRet.MinVolume = 1;	//��С�ɽ���:1	
		lRet.ContingentCondition = THOST_FTDC_CC_Immediately;  //��������:����
		//TThostFtdcPriceType	StopPrice;  //ֹ���
		lRet.ForceCloseReason = THOST_FTDC_FCC_NotForceClose;	//ǿƽԭ��:��ǿƽ	
		lRet.IsAutoSuspend = 0;  //�Զ������־:��	
		lRet.UserForceClose = 0;   //�û�ǿ����־:
		return lRetPtr;
	}

	void CTP_TD::OnRtnOrder( CThostFtdcOrderField *pOrder )
	{
		boost::shared_ptr<CThostFtdcOrderField> lpOrder (new CThostFtdcOrderField);
		memcpy(lpOrder.get(),pOrder,sizeof(CThostFtdcOrderField));
		m_DataCache.UpdataOrder(lpOrder);
		m_pTradeSpi->OnRtnOrder(BuildRtnOrderStr(lpOrder));
	}

	void CTP_TD::OnRtnTrade( CThostFtdcTradeField *pTrade )
	{
		boost::shared_ptr<CThostFtdcTradeField> lpTrade (new CThostFtdcTradeField);
		memcpy(lpTrade.get(),pTrade,sizeof(CThostFtdcTradeField));
		m_DataCache.UpdataTrade(lpTrade);
		m_pTradeSpi->OnRtnTrade(BuildRtnTradeStr(lpTrade));
		m_pTradeSpi->OnRtnState(Position_Change,m_DataCache.GeneratorPositionString());
	}

	std::string CTP_TD::BuildRtnTradeStr( boost::shared_ptr<CThostFtdcTradeField> apTrade )
	{
		std::string lExchangOrderID = GenerateExchangeOrderID(apTrade);
		std::string lThostOrderID = m_DataCache.GetThostOrderIDByExchangeOrderID(lExchangOrderID);
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
		std::string lThostOrderID = m_DataCache.GetThostOrderIDByExchangeOrderID(lExchangOrderID);
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
			m_pTradeSpi->OnRtnState(Cancel_Failed,lbuf.str());
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
			m_pTradeSpi->OnRtnState(CreateOrder_Failed,lbuf.str());
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
			m_pTradeSpi->OnRtnState(QryPosition_Failed,"QryInvestorPosition  Failed");
			m_IsInQryPosition = false; 
		}

		boost::shared_ptr<CThostFtdcInvestorPositionField>  lpPos(new CThostFtdcInvestorPositionField);
		memcpy(lpPos.get(),pInvestorPosition,sizeof(CThostFtdcInvestorPositionField));
		m_DataCache.UpdatePosition(lpPos);

		if(bIsLast)
		{
			m_IsInQryPosition = false; 
			std::string lPosRspStr = m_DataCache.GeneratorPositionString();
			m_pTradeSpi->OnRtnPosition(lPosRspStr);
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

		/////
		//TThostFtdcAccountIDType	AccountID;
		/////����ı�֤��
		//TThostFtdcMoneyType	FrozenMargin;
		/////������ʽ�
		//TThostFtdcMoneyType	FrozenCash;
		/////��ǰ��֤���ܶ�
		//TThostFtdcMoneyType	CurrMargin;

		/////ƽ��ӯ��
		//TThostFtdcMoneyType	CloseProfit;
		/////�ֲ�ӯ��
		//TThostFtdcMoneyType	PositionProfit;
		/////�ڻ�����׼����
		//TThostFtdcMoneyType	Balance;
		/////�����ʽ�
		//TThostFtdcMoneyType	Available;

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
