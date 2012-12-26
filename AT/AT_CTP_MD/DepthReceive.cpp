#include "DepthReceive.h"
#include "CTP_API.h"
#include <boost/foreach.hpp>
#include "DataCacheCTP.h"
#include <sstream>
namespace CTP
{

	DepthReceiver::DepthReceiver( const std::string aConfigStr )
	{
		m_RequestID = 0;
		m_DepthState =  DepthReceiver_UNINITIALIZE_STATE;
	}

	DepthReceiver::~DepthReceiver()
	{
		typedef std::map<std::string,char**>::_Val_type  pairType;
		BOOST_FOREACH(pairType lPair,m_allocateMemMap)
		{
			char* lstrbuf = *(lPair.second);
			delete[] lstrbuf;
			delete[] lPair.second;
		}


		if(m_pMDAPI)
		{
			m_pMDAPI->RegisterSpi(NULL);
			m_pMDAPI->Join();
		}
		m_pMDAPI->Release();
	}


	char** DepthReceiver::GenerateInstrumentList( const std::string& aInstrumentID )
	{
		if(!m_allocateMemMap.count(aInstrumentID))
		{
			char* lTmepBuf = new char[aInstrumentID.size()];
			char** ltemplist = new char* [1];
			strcpy(lTmepBuf,aInstrumentID.c_str());
			ltemplist[0] = lTmepBuf;
			m_allocateMemMap[aInstrumentID] = ltemplist;
		}
		return m_allocateMemMap[aInstrumentID];
	}

	void DepthReceiver::SetDepthReceive( CTP_MD* parent,boost::shared_ptr<DataCacheCTP> apDataCacheCTP )
	{
		m_pCTP_MD = parent;
		m_pDataCache = apDataCacheCTP;
	}

	void DepthReceiver::Start()
	{
		m_pMDAPI = CThostFtdcMdApi::CreateFtdcMdApi(".\\flow2\\");
		m_pMDAPI->RegisterSpi(this);		 
		m_pMDAPI->RegisterFront("tcp://asp-sim2-front1.financial-trading-platform.com:26213");
		m_pMDAPI->Init();
		m_DepthState = DepthReceiver_CONNECTING_STATE;
		m_pCTP_MD->NotifySubModuleState(m_DepthState);	
	}

	void DepthReceiver::OnFrontConnected()
	{
		if( DepthReceiver_CONNECTING_STATE == m_DepthState)
		{
			CThostFtdcReqUserLoginField lLoginReq;
			memset(&lLoginReq,0,sizeof(CThostFtdcReqUserLoginField));
			m_pMDAPI->ReqUserLogin(&lLoginReq,++m_RequestID);
			m_DepthState = DepthReceiver_LOGINING_STATE;
			m_pCTP_MD->NotifySubModuleState(m_DepthState,std::string(lLoginReq.UserID,15));
		}
		else
		{
			std::cerr << "Disconnect ? auto reconnect?????\n";
		}
	}
	void DepthReceiver::OnRspUserLogin(CThostFtdcRspUserLoginField *pRspUserLogin,
		CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
	{

		//TODO more work about auto reconnect whether cause  recall the flow

		if(IsErrorRspInfo(pRspInfo))
		{
			m_DepthState = DepthReceiver_ERROR_STOP_STATE;
			m_pCTP_MD->NotifySubModuleState(m_DepthState,std::string(pRspInfo->ErrorMsg,80));
			return;
		}
		else
		{
			m_DepthState = DepthReceiver_RECEIVE_STATE;
			m_pCTP_MD->NotifySubModuleState(m_DepthState);
		}
	}

	void DepthReceiver::SubscribeInstrument( const std::string& aInstrumentID )
	{	
		char** lList = GenerateInstrumentList(aInstrumentID);
		m_pMDAPI->SubscribeMarketData(lList,1);
	}
	void DepthReceiver::UnSubscribeInstrument( const std::string& aInstrumentID )
	{
		m_pMDAPI->UnSubscribeMarketData(GenerateInstrumentList(aInstrumentID),1);
	}

	void DepthReceiver::OnRspSubMarketData( CThostFtdcSpecificInstrumentField *pSpecificInstrument, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast )
	{
		if(IsErrorRspInfo(pRspInfo))
		{
			//TODO log
			std::cerr<<"OnRspSubMarketData Error "<< pSpecificInstrument->InstrumentID<<std::endl;
			m_pCTP_MD->NotifySubModuleState(DepthReceiver_ERROR_SUBSCRIBE_EVENT);
		}
	}

	void DepthReceiver::OnRspUnSubMarketData( CThostFtdcSpecificInstrumentField *pSpecificInstrument, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast )
	{
		if(IsErrorRspInfo(pRspInfo))
		{
			//TODO log
			std::cerr<<"OnRspUnSubMarketData Error "<< pSpecificInstrument->InstrumentID<<std::endl;
			m_pCTP_MD->NotifySubModuleState(DepthReceiver_ERROR_UNSUBSCRIBE_EVENT);
		}
	}

	void DepthReceiver::OnRtnDepthMarketData( CThostFtdcDepthMarketDataField *pDepthMarketData )
	{
		boost::shared_ptr<CThostFtdcDepthMarketDataField> lMakretPtr(new CThostFtdcDepthMarketDataField);
		memcpy(lMakretPtr.get(),pDepthMarketData,sizeof(CThostFtdcDepthMarketDataField));
		m_pDataCache->AddMarketTick(lMakretPtr);
		m_pCTP_MD->NotifyMarketDepth(BuildMarketDepthString(lMakretPtr));
	}

	std::string DepthReceiver::BuildMarketDepthString( boost::shared_ptr<CThostFtdcDepthMarketDataField> aMarketPtr )
	{
		std::stringstream lStringStream;
		
			lStringStream<<"名称: "<<aMarketPtr->InstrumentID
				<<" 最新价:"<<aMarketPtr->LastPrice
				<<" 最高价:" << aMarketPtr->HighestPrice
				<<" 最低价:" << aMarketPtr->LowestPrice
				<<" 卖一价:" << aMarketPtr->AskPrice1 
				<<" 卖一量:" << aMarketPtr->AskVolume1 
				<<" 买一价:" << aMarketPtr->BidPrice1
				<<"	买一量:" << aMarketPtr->BidVolume1
				<<" 市场持仓:"<< aMarketPtr->OpenInterest <<std::endl;
		std::string lRet(lStringStream.str());
		return lRet;
	}






}
