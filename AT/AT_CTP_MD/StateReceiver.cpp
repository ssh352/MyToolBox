#include "StateReceiver.h"
#include <iostream>
#include  "MyForeach.h"
#include "DataCacheCTP.h"
#include <set>
#include <boost\lexical_cast.hpp>
namespace CTP
{
	StateReceiver::StateReceiver( const std::map<std::string,std::string>& aConfigStr )
		:  m_RequestID(0)
		, m_ConfigMap(aConfigStr)
	{
		 m_RuningState =  StateReceiver_UNINITIALIZE_STATE;
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


	void StateReceiver::SetStateReceive(CTP_MD* apParent , boost::shared_ptr<DataCacheCTP> apDataCache)
	{
		m_pCTP_MD = apParent;
		m_pDataCache = apDataCache;
	}

	void StateReceiver::Start()
	{
		m_pTraderAPI = CThostFtdcTraderApi::CreateFtdcTraderApi("flow1");
		m_pTraderAPI->RegisterSpi(this);		
		m_pTraderAPI->SubscribePublicTopic(THOST_TERT_RESTART);					
		m_pTraderAPI->SubscribePrivateTopic(THOST_TERT_RESTART);			 
		m_pTraderAPI->RegisterFront("tcp://asp-sim2-front1.financial-trading-platform.com:26205");
		m_pTraderAPI->Init();
		m_RuningState = StateReceiver_CONNECTING_STATE;
		m_pCTP_MD->NotifySubModuleState(m_RuningState);
	}


	void StateReceiver::OnFrontConnected()
	{
		if( StateReceiver_CONNECTING_STATE == m_RuningState)
		{
			CThostFtdcReqUserLoginField lLoginReq;
			strcpy_s(lLoginReq.BrokerID,11,"2030");
			strcpy_s(lLoginReq.UserID,15,"000000005510");
			strcpy_s(lLoginReq.Password,41,"228636");

			int ret = m_pTraderAPI->ReqUserLogin(&lLoginReq,++m_RequestID);
			if(ret!=0) std::cerr<<"ReqUserLogin Send Failed"<<std::endl;
			m_RuningState = StateReceiver_LOGINING_STATE;
			m_pCTP_MD->NotifySubModuleState(m_RuningState,std::string(lLoginReq.UserID,15));
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
			m_RuningState = StateReceiver_ERROR_STOP_STATE;
			m_pCTP_MD->NotifySubModuleState(m_RuningState,std::string(pRspInfo->ErrorMsg,80));
			return;
		}
		else
		{
			m_RuningState = StateReceiver_RETRIEVE_EXCHANGE_STATE;
			m_pCTP_MD->NotifySubModuleState(m_RuningState);
			std::cerr<<"Start Retrieving Exchange"<<std::endl;
			CThostFtdcQryExchangeField lQryExchange;
			memset(&lQryExchange,0,sizeof(lQryExchange));
			int ret = m_pTraderAPI->ReqQryExchange(&lQryExchange,++m_RequestID);
			if(ret!=0) std::cerr<<"ReqQryExchange Send Failed"<<std::endl;
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

			boost::shared_ptr<CThostFtdcExchangeField> lpExchange(new CThostFtdcExchangeField);
			memcpy(lpExchange.get(),pExchange,sizeof(CThostFtdcExchangeField));
			m_pDataCache->AddExchange(lpExchange);
			m_pCTP_MD->NotifyExchange(std::string(pExchange->ExchangeID,8));
			if(bIsLast)
			{
				m_RuningState = StateReceiver_RETRIEVE_PRODUCT_STATE;
				m_pCTP_MD->NotifySubModuleState(m_RuningState);

				std::cerr<<"Start Retrieving Instrument the Product will not Retrieve separate"<<std::endl;
				CThostFtdcQryInstrumentField lQryInstrument;
				memset(&lQryInstrument,0,sizeof(lQryInstrument));
				int ret =  m_pTraderAPI->ReqQryInstrument(&lQryInstrument,++m_RequestID);
				if(ret!=0) std::cerr<<"ReqQryInstrument Send Failed"<<std::endl;
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
			if( StateReceiver_RETRIEVE_PRODUCT_STATE == m_RuningState )
			{
				boost::shared_ptr<CThostFtdcInstrumentField> lpInstrument(new CThostFtdcInstrumentField);
				memcpy(lpInstrument.get(),pInstrument,sizeof(CThostFtdcInstrumentField));
				m_pDataCache->AddInstrument(lpInstrument);
			}

			if(bIsLast)
			{
				std::set<std::string> lProductList = m_pDataCache->GetProductList();
				BOOST_FOREACH(std::string lProductID,lProductList )
				{
					m_pCTP_MD->NotifyProduct(lProductID);
				}
				
				std::cerr<<"Finish Retrieving Instrument the Product "<<std::endl;
				m_RuningState = StateReceiver_RETRIEVE_INSTRUMENT_STATE;
				m_pCTP_MD->NotifySubModuleState(m_RuningState);

				std::vector<std::string> lInstrumentList = m_pDataCache->GetInstrumentListAll();
				BOOST_FOREACH( std::string InstrumentID,lInstrumentList )
				{
					m_pCTP_MD->NotifyInstrument(InstrumentID);
				}
				m_RuningState = StateReceiver_RETRIEVE_DYNAMIC_STATE ;
				m_pCTP_MD->NotifySubModuleState(m_RuningState);
			}
		}

	}

	void StateReceiver::OnRtnInstrumentStatus( CThostFtdcInstrumentStatusField *pInstrumentStatus )
	{
		//todo when Trading phase
	}

	void StateReceiver::OnFrontDisconnected( int nReason )
	{
		std::string ErrMessage;
		switch(nReason)
		{
			case 0x1001:
				ErrMessage = "网络读失败";
				break;
			case 0x1002:
				ErrMessage = "网络写失败";
				break;
			case 0x2001:
				ErrMessage = "接收心跳超时";
				break;
			case 0x2002:
				ErrMessage = "发送心跳失败";
				break;
			case 0x2003:
				ErrMessage = "收到错误报文";
				break;
			default:
				ErrMessage = "其他错误序号";
				ErrMessage += boost::lexical_cast<std::string>(nReason);
				break;
		}
		std::cerr<<"State======================= 断开连接 "<<ErrMessage<<std::endl;
	}






}

