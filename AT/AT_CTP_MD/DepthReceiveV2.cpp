#include "DepthReceiveV2.h"

#include <sstream>
#include <iostream>
#include <boost/lexical_cast.hpp>

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>

using namespace AT;
namespace CTP
{


	DepthReceiverV2::DepthReceiverV2( const std::string aConfigXml ,MarketHandlerFun aHandle )
		: m_Markethandle(aHandle)
		, m_RequestID(1)
	{
		std::stringstream lbuf(aConfigXml);
		using boost::property_tree::ptree;
		ptree pt;
		read_xml(lbuf,pt);

		for(auto lVale: pt.get_child("MDConfig.InstrumentList"))
		{
			m_SubcribeList.insert(lVale.second.data());
		}

		m_Front = pt.get<std::string>("MDConfig.Front");
		m_WorkFlowDir =  pt.get<std::string>("MDConfig.WorkFlowDir");
	}

	void DepthReceiverV2::Start()
	{
		//TODO check the Dir if not exist ,create it
		m_pMDAPI = CThostFtdcMdApi::CreateFtdcMdApi(/*m_WorkFlowDir.c_str()*/);
		m_pMDAPI->RegisterSpi(this);
		char buf_front[256];
		strcpy_s(buf_front,sizeof(buf_front),m_Front.c_str());
		m_pMDAPI->RegisterFront(buf_front);
		m_pMDAPI->Init();
	}

	void DepthReceiverV2::Stop()
	{
		if(m_pMDAPI)
		{
			m_pMDAPI->RegisterSpi(nullptr);
			m_pMDAPI->Join();
		}
		m_pMDAPI->Release();
	}

	void DepthReceiverV2::OnFrontConnected()
	{
		CThostFtdcReqUserLoginField lLoginReq;
		memset(&lLoginReq,0,sizeof(CThostFtdcReqUserLoginField));
		m_pMDAPI->ReqUserLogin(&lLoginReq,++m_RequestID);
	}
	void DepthReceiverV2::OnRspUserLogin(CThostFtdcRspUserLoginField *pRspUserLogin,
		CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
	{
		//TODO more work about auto reconnect whether cause  recall the flow
		if(IsErrorRspInfo(pRspInfo))
		{
			return;
		}
		else
		{
			SubscribeList();
		}
	}
	void DepthReceiverV2::SubscribeList()
	{
		char buf[10] = {0};
		char* lList[1] ={0};
		for(std::string lInstrumen : m_SubcribeList)
		{
			strcpy_s(buf,sizeof(buf),lInstrumen.c_str());
			lList[0] = buf;
			m_pMDAPI->SubscribeMarketData(lList,1);
		}
	}


	void DepthReceiverV2::OnRspSubMarketData( CThostFtdcSpecificInstrumentField *pSpecificInstrument, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast )
	{
		if(IsErrorRspInfo(pRspInfo))
		{
			std::cerr<<"OnRspSubMarketData Error "<< pSpecificInstrument->InstrumentID<<std::endl;
		}
	}


	void DepthReceiverV2::OnRtnDepthMarketData( CThostFtdcDepthMarketDataField *pDepthMarketData )
	{

		AT::MarketData lBuildMarket = Build_AT_Market(pDepthMarketData);
		m_Markethandle(lBuildMarket);
	}

	void DepthReceiverV2::OnFrontDisconnected( int nReason )
	{
		std::string ErrMessage;
		switch(nReason)
		{
		case 0x1001:
			ErrMessage = "ÍøÂç¶ÁÊ§°Ü";
			break;
		case 0x1002:
			ErrMessage = "ÍøÂçÐ´Ê§°Ü";
			break;
		case 0x2001:
			ErrMessage = "½ÓÊÕÐÄÌø³¬Ê±";
			break;
		case 0x2002:
			ErrMessage = "·¢ËÍÐÄÌøÊ§°Ü";
			break;
		case 0x2003:
			ErrMessage = "ÊÕµ½´íÎó±¨ÎÄ";
			break;
		default:
			ErrMessage = "ÆäËû´íÎóÐòºÅ";
			ErrMessage += boost::lexical_cast<std::string>(nReason);
			break;
		}
		std::cerr<<"Depth===================== ¶Ï¿ªÁ¬½Ó "<<ErrMessage<<std::endl;
	}

	bool DepthReceiverV2::IsValidPrice( double aPrice )
	{
		//note 1.79769e+308; is the max val .
		return aPrice < 1.79768e+308;
	}

	double DepthReceiverV2::GetDispalyPrice( double aPrice )
	{
		return IsValidPrice(aPrice)? aPrice: -1;
	}

	DepthReceiverV2::~DepthReceiverV2()
	{

	}

	AT::MarketData DepthReceiverV2::Build_AT_Market( CThostFtdcDepthMarketDataField* aMarketPtr )
	{
		AT::MarketData lRet;

		memcpy(lRet.InstrumentID,aMarketPtr->InstrumentID,sizeof(aMarketPtr->InstrumentID));
		AT::AT_Time lTime = Build_AT_Time( aMarketPtr->UpdateTime, aMarketPtr->UpdateMillisec);

		lRet.m_LastPrice =  IsValidPrice( aMarketPtr->LastPrice)?  TranPriceToInt( aMarketPtr->LastPrice) : AT_INVALID_PRICE;
		lRet. m_BidPrice =  IsValidPrice( aMarketPtr->BidPrice1)? TranPriceToInt( aMarketPtr->BidPrice1) : AT_INVALID_PRICE;
		lRet. m_AskPrice  =  IsValidPrice( aMarketPtr->AskPrice1)? TranPriceToInt( aMarketPtr->AskPrice1) : AT_INVALID_PRICE;
		lRet.m_trunover = IsValidPrice( aMarketPtr->AskPrice1)?  TranPriceToInt( aMarketPtr->Turnover):AT_INVALID_PRICE;
		lRet.m_AskVol  = aMarketPtr->AskVolume1;
		lRet. m_BidVol  = aMarketPtr->BidVolume1;

		return std::move(lRet);

	}

	AT::AT_Time DepthReceiverV2::Build_AT_Time( TThostFtdcTimeType aSecond, int millsecond )
	{
		return AT::AT_INVALID_TIME;
	}


}
