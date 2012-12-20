#pragma once
#include "CTP_API.h"
#include "CTP_MD.h"
#include <string>
#include <vector>
#include <map>
namespace CTP
{
	class StateReceiver :public CThostFtdcTraderSpi
	{
	public:
		StateReceiver(const std::string aConfigStr);
		~StateReceiver();
		void SetStateReceive(CTP_MD* parent);
		void Start();

		//from CTP api
	public:
		virtual void OnFrontConnected();
		virtual void OnRspUserLogin(CThostFtdcRspUserLoginField *apRspUserLogin,
			CThostFtdcRspInfoField *apRspInfo, int anRequestID, bool abIsLast);
		virtual void OnRspQryExchange(CThostFtdcExchangeField *pExchange, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);
		virtual void OnRspQryInstrument(CThostFtdcInstrumentField *pInstrument, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);


	private:
		bool IsErrorRspInfo(CThostFtdcRspInfoField *pRspInfo);

	private:
		enum StateReceiver_State
		{
			UNINITIALIZE_STATE,
			CONNECTING_STATE,
			LOGINING_STATE,
			RETRIEVE_EXCHANGE_STATE,
			RETRIEVE_PRODUCT_STATE,
			RETRIEVE_INSTRUMENT_STATE,
			RETRIEVE_DYNAMIC_STATE,
			ERROR_DISCONNECT_STATE,
			ERROR_STOP_STATE
		};


		CThostFtdcTraderApi*  m_pTraderAPI;
		CTP_MD*	m_pCTP_MD;
		int		m_RequestID;
		StateReceiver_State	m_RuningState;

		typedef std::vector< boost::shared_ptr<CThostFtdcInstrumentField> > InstrumentVec;

		InstrumentVec m_InstrumentVec;
		std::map<std::string,InstrumentVec > m_ProductMap;
	};

}

