#include "StateReceiver.h"
#include <iostream>

namespace CTP
{
	StateReceiver::StateReceiver( const std::string aConfigStr )
		: m_isAlreadyLogin(false)
		, m_RequestID(0)
	{

	}

	StateReceiver::~StateReceiver()
	{
		if(m_pTraderAPI)
		{
			m_pTraderAPI->RegisterSpi(NULL);
			m_pTraderAPI->Join();
		}
		m_pTraderAPI->Release();
	}


	void StateReceiver::SetStateReceive( CTP_MD* apParent)
	{
		m_pCTP_MD = apParent;
	}

	void StateReceiver::Start()
	{
		char lConPath[128] = "./MD_StateReceiver/";
		m_pTraderAPI = CThostFtdcTraderApi::CreateFtdcTraderApi();
		m_pTraderAPI->RegisterSpi(this);		
		m_pTraderAPI->SubscribePublicTopic(THOST_TERT_RESTART);					
		m_pTraderAPI->SubscribePrivateTopic(THOST_TERT_RESTART);			 
		m_pTraderAPI->RegisterFront("tcp://asp-sim2-front1.financial-trading-platform.com:26205");
		m_pTraderAPI->Init();
		m_pCTP_MD->NotifySubModuleState(CTP_MD_StateReceiver_Connecting);
	}

	bool StateReceiver::IsErrorRspInfo( CThostFtdcRspInfoField *pRspInfo )
	{
		bool ret = ((pRspInfo) && (pRspInfo->ErrorID != 0));
		if(ret)
		{
			//TODO log error
		}
		return ret;
	}

	void StateReceiver::OnFrontConnected()
	{
		if(!m_isAlreadyLogin)
		{
			CThostFtdcReqUserLoginField lLoginReq;
			m_pTraderAPI->ReqUserLogin(&lLoginReq,++m_RequestID);
			m_isAlreadyLogin = true;
			m_pCTP_MD->NotifySubModuleState(CTP_MD_StateReceiver_Logining,std::string(lLoginReq.UserID,15));
		}
	}
	void StateReceiver::OnRspUserLogin(CThostFtdcRspUserLoginField *pRspUserLogin,
		CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
	{

		//TODO more work about auto reconnect whether cause  recall the flow

		if(IsErrorRspInfo(pRspInfo))
		{
			m_pCTP_MD->NotifySubModuleState(CTP_MD_StateReceiver_Login_Failed,std::string(pRspInfo->ErrorMsg,80));
			return;
		}
		else
		{
			m_pCTP_MD->NotifySubModuleState(CTP_MD_StateReceiver_Retrieving);
			std::cerr<<"Start Retrieving Exchange"<<std::endl;
			CThostFtdcQryExchangeField lQryExchange;
			memset(&lQryExchange,0,sizeof(lQryExchange));
			m_pTraderAPI->ReqQryExchange(&lQryExchange,++m_RequestID);
		}

		////assume login rsp will only return once per login
		//if(!bIsLast)
		//{
		//	std::cerr<<"Waring StateReceiver Receive more than 1 OnRspUserLogin"<<std::endl;
		//	m_pCTP_MD->NotifySubModuleState(CTP_MD_StateReceiver_Login_Failed,std::string(pRspInfo->ErrorMsg,80));
		//}
	}

	void StateReceiver::OnRspQryExchange( CThostFtdcExchangeField *pExchange, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast )
	{
		if(IsErrorRspInfo(pRspInfo))
		{
			m_pCTP_MD->NotifySubModuleState(CTP_MD_StateReceiver_Retrieve_Failed,std::string(pRspInfo->ErrorMsg,80));
		}
		else
		{
			//todo build xml text for more info but now ,just the ID send out
			m_pCTP_MD->NotifyExchange(std::string(pExchange->ExchangeID,8));
			if(bIsLast)
			{
				std::cerr<<"Start Retrieving Instrument the Product will not Retrieve separate"<<std::endl;
				//CThostFtdcQryInstrumentField lQryInstrument;
				//memset(&lQryInstrument,0,sizeof(lQryInstrument));
				//m_pTraderAPI->ReqQryInstrument(&lQryProduct,++m_RequestID);
			}
		}


	}





}

