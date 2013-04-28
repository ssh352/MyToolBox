#pragma once

#include <string>
#include <memory>
#include <set>
#include <functional>
#include "CTP_API.h"
#include "AT_Struct.h"
namespace CTP
{

	//this class should using for Receive MD tick and Notify Out Side Info

	typedef std::function<void (std::shared_ptr< AT::MarketData> )> MarketHandlerFun; 

	//ConfigTemplate
	//MDConfig.Front
	//MDConfig.InstrumentList
	//MDConfig.WorkFlowDir
	//MDConfig.StroeMarketDir


	class DepthReceiverV2 :public CThostFtdcMdSpi
	{
	public:
		DepthReceiverV2(const std::string aConfigXml ,MarketHandlerFun aHandle);
		~DepthReceiverV2();
		void Start();
		void Stop();
	private:
		void SubscribeList();
		//std::string BuildMarketDepthString(CThostFtdcDepthMarketDataField* aMarketPtr);
		std::shared_ptr<AT::MarketData> Build_AT_Market(CThostFtdcDepthMarketDataField* aMarketPtr);
		AT::AT_Time Build_AT_Time(TThostFtdcTimeType, int millsecond);

	public: //from CTP API
		virtual void OnFrontConnected();
		virtual void OnRspUserLogin(CThostFtdcRspUserLoginField *pRspUserLogin,
			CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);
		virtual void OnRspSubMarketData(CThostFtdcSpecificInstrumentField *pSpecificInstrument, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);
		virtual void OnRtnDepthMarketData(CThostFtdcDepthMarketDataField *pDepthMarketData);
		
		virtual void OnFrontDisconnected(int nReason);

		
	private://Manager Self
		CThostFtdcMdApi*  m_pMDAPI;
		int m_RequestID;
		bool IsValidPrice(double aPrice);
		double  GetDispalyPrice(double aPrice);

		
	private://Get From OutSide
		std::string m_Front;
		std::string m_WorkFlowDir;
		std::set<std::string> m_SubcribeList;
		MarketHandlerFun m_Markethandle;
	};

}


