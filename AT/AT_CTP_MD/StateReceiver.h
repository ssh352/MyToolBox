#pragma once
#include "CTP_API.h"
#include "CTP_MD.h"
#include <string>
#include <vector>
#include <map>

namespace CTP
{
	class DataCacheCTP;

	class StateReceiver :public CThostFtdcTraderSpi
	{
	public:
		StateReceiver(const std::string aConfigStr);
		~StateReceiver();
		void SetStateReceive(CTP_MD* parent , boost::shared_ptr<DataCacheCTP> apDataCache);
		void Start();

		//from CTP api
	public:
		virtual void OnFrontConnected();
		virtual void OnFrontDisconnected(int nReason);
		virtual void OnRspUserLogin(CThostFtdcRspUserLoginField *apRspUserLogin,
			CThostFtdcRspInfoField *apRspInfo, int anRequestID, bool abIsLast);
		virtual void OnRspQryExchange(CThostFtdcExchangeField *pExchange, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);
		virtual void OnRspQryInstrument(CThostFtdcInstrumentField *pInstrument, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);
		virtual void OnRtnInstrumentStatus(CThostFtdcInstrumentStatusField *pInstrumentStatus);
	private:
	


		CThostFtdcTraderApi*  m_pTraderAPI;
		CTP_MD*	m_pCTP_MD;
		int		m_RequestID;
		MD_SubMoudle_State	m_RuningState;

		//typedef std::vector< boost::shared_ptr<CThostFtdcInstrumentField> > InstrumentVec;

		//InstrumentVec m_InstrumentVec;
		//std::map<std::string,InstrumentVec > m_ProductMap;
		boost::shared_ptr<DataCacheCTP> m_pDataCache;
	};

}

