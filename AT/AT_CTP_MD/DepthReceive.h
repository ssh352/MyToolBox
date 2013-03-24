#pragma once

#include <string>
#include <boost\shared_ptr.hpp>
#include "CTP_API.h"
#include "CTP_MD.h"
#include <map>

namespace CTP
{
	class DataCacheCTP;
	class CTP_MD;
	class DepthReceiver :public CThostFtdcMdSpi
	{
	public:
		DepthReceiver(const std::map<std::string,std::string>& aConfigStr);
		~DepthReceiver();
		void SetDepthReceive(CTP_MD* parent,boost::shared_ptr<DataCacheCTP> apDataCacheCTP);
		void Start();
		void SubscribeInstrument(const std::string& aInstrumentID );
		void UnSubscribeInstrument(const std::string& aInstrumentID);

	private:
		char** GenerateInstrumentList(const std::string& aInstrumentID);
		std::string BuildMarketDepthString(boost::shared_ptr<CThostFtdcDepthMarketDataField> aMarketPtr);
		void ReplayAllMarketData();

	public: //from CTP API
		virtual void OnFrontConnected();
		virtual void OnRspUserLogin(CThostFtdcRspUserLoginField *pRspUserLogin,
			CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

		virtual void OnRspSubMarketData(CThostFtdcSpecificInstrumentField *pSpecificInstrument, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);
		virtual void OnRspUnSubMarketData(CThostFtdcSpecificInstrumentField *pSpecificInstrument, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);
		virtual void OnRtnDepthMarketData(CThostFtdcDepthMarketDataField *pDepthMarketData);
		virtual void OnFrontDisconnected(int nReason);
	private:
		CTP_MD*	m_pCTP_MD;
		boost::shared_ptr<DataCacheCTP> m_pDataCache;
		CThostFtdcMdApi*  m_pMDAPI;
		MD_SubMoudle_State m_DepthState;
		int m_RequestID;
		std::map<std::string,char**>  m_allocateMemMap;
		const std::map<std::string,std::string> m_ConfigMap;
		bool IsValidPrice(double aPrice);
		double  GetDispalyPrice(double aPrice);
	};

}


