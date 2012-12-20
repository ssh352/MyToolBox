#include "StateReceiver.h"
#include <iostream>
#include <boost/foreach.hpp>
namespace CTP
{
	StateReceiver::StateReceiver( const std::string aConfigStr )
		:  m_RequestID(0)
		
	{
		 m_RuningState =  UNINITIALIZE_STATE;
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
		m_RuningState = CONNECTING_STATE;
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
		if( CONNECTING_STATE == m_RuningState)
		{
			CThostFtdcReqUserLoginField lLoginReq;
			strcpy_s(lLoginReq.BrokerID,11,"2030");
			strcpy_s(lLoginReq.UserID,15,"000000005510");
			strcpy_s(lLoginReq.Password,41,"228636");

			m_pTraderAPI->ReqUserLogin(&lLoginReq,++m_RequestID);
			m_RuningState = LOGINING_STATE;
			m_pCTP_MD->NotifySubModuleState(CTP_MD_StateReceiver_Logining,std::string(lLoginReq.UserID,15));
		}
		else
		{
			//m_RuningState = ERROR_STOP_STATE;
			std::cerr << "Disconnect ? auto reconnect?????\n";
		}
	}
	void StateReceiver::OnRspUserLogin(CThostFtdcRspUserLoginField *pRspUserLogin,
		CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
	{

		//TODO more work about auto reconnect whether cause  recall the flow

		if(IsErrorRspInfo(pRspInfo))
		{
			m_RuningState = ERROR_STOP_STATE;
			m_pCTP_MD->NotifySubModuleState(CTP_MD_StateReceiver_Login_Failed,std::string(pRspInfo->ErrorMsg,80));
			return;
		}
		else
		{
			m_RuningState = RETRIEVE_EXCHANGE_STATE;
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
				m_RuningState = RETRIEVE_PRODUCT_STATE;
				std::cerr<<"Start Retrieving Instrument the Product will not Retrieve separate"<<std::endl;
				CThostFtdcQryInstrumentField lQryInstrument;
				memset(&lQryInstrument,0,sizeof(lQryInstrument));
				m_pTraderAPI->ReqQryInstrument(&lQryInstrument,++m_RequestID);
			}
		}
	}

	void StateReceiver::OnRspQryInstrument( CThostFtdcInstrumentField *pInstrument, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast )
	{
		if(IsErrorRspInfo(pRspInfo))
		{
			m_pCTP_MD->NotifySubModuleState(CTP_MD_StateReceiver_Retrieve_Failed,std::string(pRspInfo->ErrorMsg,80));
		}
		else
		{
			if( RETRIEVE_PRODUCT_STATE == m_RuningState )
			{
				boost::shared_ptr<CThostFtdcInstrumentField> lpInstrument(new CThostFtdcInstrumentField);
				memcpy(lpInstrument.get(),pInstrument,sizeof(pInstrument));
				m_InstrumentVec.push_back(lpInstrument);

				std::string lProductID(lpInstrument->ProductID,30); 
				std::map<std::string,InstrumentVec >::iterator iter = m_ProductMap.find(lProductID);
				if(iter != m_ProductMap.end() )
				{
					iter->second.push_back(lpInstrument);
				}
				else
				{
					InstrumentVec lVec;
					lVec.push_back(lpInstrument);
					m_ProductMap.insert(make_pair(lProductID,lVec));
					m_pCTP_MD->NotifyProduct(lProductID);
				}
			}

			if(bIsLast)
			{
				std::cerr<<"Finish Retrieving Instrument the Product "<<std::endl;
				m_RuningState = RETRIEVE_INSTRUMENT_STATE;
				m_pCTP_MD->NotifySubModuleState(m_RuningState);
				BOOST_FOREACH( boost::shared_ptr<CThostFtdcInstrumentField> lpInstrument,m_InstrumentVec )
				{
					std::string lInstrumentID (lpInstrument->InstrumentID,30);
					m_pCTP_MD->NotifyInstrument(lInstrumentID);
				}
				m_RuningState = RETRIEVE_DYNAMIC_STATE ;
				m_pCTP_MD->NotifySubModuleState(m_RuningState);
			}
		}

	}






}

