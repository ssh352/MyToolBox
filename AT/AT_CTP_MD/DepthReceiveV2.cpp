#include "DepthReceiveV2.h"

#include <sstream>
#include <iostream>
#include <boost/lexical_cast.hpp>

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>
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
		m_pMDAPI = CThostFtdcMdApi::CreateFtdcMdApi(m_WorkFlowDir.c_str());
		m_pMDAPI->RegisterSpi(this);
		char buf_front[256];
		strcpy(buf_front,m_Front.c_str());
		m_pMDAPI->RegisterFront(buf_front);
		m_pMDAPI->Init();
	}

	void DepthReceiverV2::Stop()
	{
		if(m_pMDAPI)
		{
			m_pMDAPI->RegisterSpi(NULL);
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
			strcpy(buf,lInstrumen.c_str());
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
		std::string lMarketString = BuildMarketDepthString(pDepthMarketData);
		m_Markethandle(lMarketString);
	}

	std::string DepthReceiverV2::BuildMarketDepthString( CThostFtdcDepthMarketDataField* aMarketPtr )
	{
		using boost::property_tree::ptree;
		ptree pt;
		pt.put("head.type","MarketUpdate");
		pt.put("head.version",0.1f);
		pt.put("market.ID" , aMarketPtr->InstrumentID );
		pt.put("market.Second" , aMarketPtr->UpdateTime );
		pt.put("market.Millsecond" , aMarketPtr->UpdateMillisec );
		pt.put("market.LastPx" , aMarketPtr->LastPrice );
		pt.put("market.HighPx" , aMarketPtr->HighestPrice );
		pt.put("market.LowPx" , aMarketPtr->LowestPrice );
		pt.put("market.AskPx1" , aMarketPtr->AskPrice1 );
		pt.put("market.AskVol1" , aMarketPtr->AskVolume1 );
		pt.put("market.BidPx1" , aMarketPtr->BidPrice1 );
		pt.put("market.BidVol1" , aMarketPtr->BidVolume1 );
		pt.put("market.OpenInterest" , aMarketPtr->OpenInterest );

		std::stringstream lStringStream;
		write_xml(lStringStream,pt);
		std::string  lRet = lStringStream.str();
		return lRet;
	}

	void DepthReceiverV2::OnFrontDisconnected( int nReason )
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

	bool DepthReceiverV2::IsValidPrice( double aPrice )
	{
		//note 1.79769e+308; is the max val .
		return aPrice < 1.79768e+308;
	}

	double DepthReceiverV2::GetDispalyPrice( double aPrice )
	{
		return IsValidPrice(aPrice)? aPrice: -1;
	}


}
