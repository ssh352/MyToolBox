#pragma once


#include "CTP_API.h"
#include "AT_Struct.h"

#include <string>
#include <memory>
#include <set>
#include <functional>
#include <thread>
//#include <boost\system\error_code.hpp>
#include <boost\asio\io_service.hpp>
#include <boost\asio\deadline_timer.hpp>



namespace CTP
{

	//this class should using for Receive MD tick and Notify Out Side Info


	enum class CTP_Market_Status_Enum
	{
		E_CTP_MD_CONNECTING,
		E_CTP_MD_LOGIN,
		E_CTP_MD_READY,
		E_CTP_MD_MARKET_DELAY,
		E_CTP_MD_MARKET_LONG_DELAY
	};
	typedef std::function<void (std::shared_ptr< AT::MarketData> )> MarketHandlerFun; 
	typedef std::function<void (CTP_Market_Status_Enum aStatus,std::string aErrorMsg)> MarketStateHandle;


	class DepthReceiveV2 :public CThostFtdcMdSpi
	{
	public:
		DepthReceiveV2(const std::string aConfigXml ,MarketHandlerFun aHandle, MarketStateHandle aStateHanle);
		~DepthReceiveV2();
		void Start();
		void Stop();
	private:
		void SubscribeList();
		//std::string BuildMarketDepthString(CThostFtdcDepthMarketDataField* aMarketPtr);
		std::shared_ptr<AT::MarketData> Build_AT_Market(CThostFtdcDepthMarketDataField* aMarketPtr);
		AT::AT_Time Build_AT_Time(TThostFtdcTimeType, int millsecond);
		void CheckMarketDelay(const boost::system::error_code& /*e*/);


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
		void ResetDelayTimer();
		void InitDelayTimer();
		void StopDelayTimer();

		
		std::thread m_CheckDelayThread;
		std::unique_ptr<boost::asio::io_service> m_pBoostIOService;
		std::unique_ptr<boost::asio::io_service::work> m_pBoostWorker;
		std::unique_ptr<boost::asio::deadline_timer> m_pCheckDelayTimer;


		
	private://Get From OutSide
		std::string m_Front;
		std::string m_WorkFlowDir;
		std::set<std::string> m_SubcribeList;
		MarketHandlerFun m_Markethandle;
		MarketStateHandle m_MarketStateHandle;

	};

}


