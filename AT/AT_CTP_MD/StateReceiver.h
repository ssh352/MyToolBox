#pragma once
#include "CTP_API.h"
#include "CTP_MD.h"
#include <string>

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


	private:
		bool IsErrorRspInfo(CThostFtdcRspInfoField *pRspInfo);

	private:
		CThostFtdcTraderApi*  m_pTraderAPI;
		CTP_MD*	m_pCTP_MD;
		bool	m_isAlreadyLogin;
		int		m_RequestID;
		bool	m_isRuning;
	};

}

