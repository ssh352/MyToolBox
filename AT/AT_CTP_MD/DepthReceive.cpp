#include "DepthReceive.h"
#include "CTP_API.h"
#include  "MyForeach.h"
#include "DataCacheCTP.h"
#include <sstream>
#include <iostream>
#include <boost/lexical_cast.hpp>
namespace CTP
{

	DepthReceiver::DepthReceiver(const std::map<std::string,std::string>& aConfigStr )
		: m_ConfigMap(aConfigStr)
		, m_RequestID(0)
	{
		m_DepthState =  DepthReceiver_UNINITIALIZE_STATE;
	}

	DepthReceiver::~DepthReceiver()
	{

		if(m_pMDAPI)
		{
			m_pMDAPI->RegisterSpi(NULL);
			m_pMDAPI->Join();
		}
		m_pMDAPI->Release();

		MYFOREACH(lPair , m_allocateMemMap)
		{
			char* lstrbuf = *(lPair.second);
			delete[] lstrbuf;
			delete[] lPair.second;
		}
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
		if(m_ConfigMap["IsReplay"] == "1")
		{
			ReplayAllMarketData();
			return ;
		}

		m_pMDAPI = CThostFtdcMdApi::CreateFtdcMdApi("flow2");
		m_pMDAPI->RegisterSpi(this);


		//lMDConfigMap["BrokerID"] =lBorkerid ;
		//lMDConfigMap["UserID"] = lUserID;
		//lMDConfigMap["Password"]=lPassWord;
		//lMDConfigMap["Front"] = lFront;
		
		//119.4.167.60:62213 华西联通MD
		
		m_pMDAPI->RegisterFront("tcp://182.131.17.110:62213"); //182.131.17.110:62213 华西电信MD
		//m_pMDAPI->RegisterFront("tcp://asp-sim2-front1.financial-trading-platform.com:26213"); // "" CTP官方
		char buf_front[256];
		strcpy(buf_front,m_ConfigMap["Front"].c_str());
		//m_pMDAPI->RegisterFront(buf_front);
		m_pMDAPI->Init();
		m_DepthState = DepthReceiver_CONNECTING_STATE;
		m_pCTP_MD->NotifySubModuleState(m_DepthState);	
	}

	void DepthReceiver::OnFrontConnected()
	{
		CThostFtdcReqUserLoginField lLoginReq;
		memset(&lLoginReq,0,sizeof(CThostFtdcReqUserLoginField));
		m_pMDAPI->ReqUserLogin(&lLoginReq,++m_RequestID);
		m_DepthState = DepthReceiver_LOGINING_STATE;
		m_pCTP_MD->NotifySubModuleState(m_DepthState,std::string(lLoginReq.UserID,15));

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
			ReplayAllMarketData();
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
			
			//lStringStream<<'@'<<aMarketPtr->InstrumentID 
			//	<<" 时间："<'@'<<<aMarketPtr->UpdateTime<<'.'<<aMarketPtr->UpdateMillisec 
			//	<<" 最新价:"<<'@'<< GetDispalyPrice(aMarketPtr->LastPrice) 
			//	<<" 最高价:" <<'@'<< GetDispalyPrice(aMarketPtr->HighestPrice) 
			//	<<" 最低价:" <<'@'<< GetDispalyPrice (aMarketPtr->LowestPrice) 
			//	<<" 卖一价:" <<'@'<< GetDispalyPrice( aMarketPtr->AskPrice1) 
			//	<<" 卖一量:" << '@'<<aMarketPtr->AskVolume1  
			//	<<" 买一价:" <<'@'<< GetDispalyPrice(aMarketPtr->BidPrice1) 
			//	<<"	买一量:" <<'@'<< aMarketPtr->BidVolume1 
			//	<<" 市场持仓:"<<'@'<< aMarketPtr->OpenInterest <<std::endl;

			lStringStream<<aMarketPtr->InstrumentID <<' '
				<<aMarketPtr->UpdateTime<<' '
				<<aMarketPtr->UpdateMillisec  <<' '
				<< GetDispalyPrice(aMarketPtr->LastPrice)  <<' '
				 << GetDispalyPrice(aMarketPtr->HighestPrice)  <<' '
				 << GetDispalyPrice (aMarketPtr->LowestPrice)  <<' '
				 << GetDispalyPrice( aMarketPtr->AskPrice1)  <<' '
				 <<aMarketPtr->AskVolume1   <<' '
				 << GetDispalyPrice(aMarketPtr->BidPrice1)  <<' '
				<< aMarketPtr->BidVolume1  <<' '
				 << aMarketPtr->OpenInterest  <<' '<<std::endl;


		std::string lRet(lStringStream.str());
		return lRet;
	}

	void DepthReceiver::OnFrontDisconnected( int nReason )
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
		std::cerr<<"Depth===================== 断开连接 "<<ErrMessage<<std::endl;
	}

	bool DepthReceiver::IsValidPrice( double aPrice )
	{
		//note 1.79769e+308; is the max val .
		return aPrice < 1.79768e+308;
	}

	double DepthReceiver::GetDispalyPrice( double aPrice )
	{
		return IsValidPrice(aPrice)? aPrice: -1;
	}

	void DepthReceiver::ReplayAllMarketData()
	{
		for ( auto lDepthIter = m_pDataCache->GetMarketDepthCache()->begin(); lDepthIter!=m_pDataCache->GetMarketDepthCache()->end();lDepthIter++)
		{
			m_pCTP_MD->NotifyMarketDepth(BuildMarketDepthString(lDepthIter->second)); 
		}
		std::cout<<"Finished_________SYNC";
	}






}
