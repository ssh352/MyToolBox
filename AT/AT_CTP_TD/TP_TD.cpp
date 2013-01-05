#include "TP_TD.h"
#include <iostream>
#include<boost\lexical_cast.hpp>
#include "ITradeSpi.h"
namespace CTP
{
	CTP_TD::CTP_TD(void)
		: m_RequestID(0)
		, m_RuningState(UnInit)
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
		m_pTraderAPI->RegisterFront("tcp://asp-sim2-front1.financial-trading-platform.com:26205");
		m_pTraderAPI->Init();

		m_RuningState = Connecting;
		 NotifyState();
	}

	void CTP_TD::OnFrontConnected()
	{
		CThostFtdcReqUserLoginField lLoginReq;
		//strcpy_s(lLoginReq.BrokerID,11,"2030");
		//strcpy_s(lLoginReq.UserID,15,"000000005510");
		//strcpy_s(lLoginReq.Password,41,"228636");

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

}
