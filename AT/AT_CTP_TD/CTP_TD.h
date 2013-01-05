#pragma once
#include <map>
#include <string>
#include "IDriver_TD.h"
#include "CTP_API.h"
namespace AT
{
	class ITradeSpi;
}
namespace CTP
{
	class CTP_TD_Imp;
	enum CTP_TD_CODE
	{
		UnInit,
		Connecting,
		Logining,
		LoginFail,
		ConfirmingSettlement,
		Ready,
	};

	class CTP_TD :public AT::IDriver_TD
		,public CThostFtdcTraderSpi
	{
	public:
		CTP_TD();
		virtual ~CTP_TD();
		virtual void Init(const std::map<std::string,std::string>& aConfigMap,AT::ITradeSpi* apTradeSpi) ;
	public:
		virtual std::string CreateOrder(const std::string& aNewOrder) ;
		virtual void DeleteOrder(const std::string& aClientOrderID) ;
		virtual	void ModifyOrder(const std::string& aRequest);
		virtual void QueryPosition(const std::string& aRequest);

		//from CTP not for user call
	public:
		virtual void OnFrontConnected();
		virtual void OnFrontDisconnected(int nReason);
		virtual void OnRspUserLogin(CThostFtdcRspUserLoginField *apRspUserLogin,
			CThostFtdcRspInfoField *apRspInfo, int anRequestID, bool abIsLast);
		virtual void OnRspQrySettlementInfoConfirm(CThostFtdcSettlementInfoConfirmField *pSettlementInfoConfirm,
			CThostFtdcRspInfoField *apRspInfo, int nRequestID, bool bIsLast) ;
		virtual void OnRspSettlementInfoConfirm(CThostFtdcSettlementInfoConfirmField *pSettlementInfoConfirm, 
			CThostFtdcRspInfoField *apRspInfo, int nRequestID, bool bIsLast);



	private:
		void  NotifyState();

	private:
		CThostFtdcTraderApi*  m_pTraderAPI;
		int m_RequestID;
		AT::ITradeSpi* m_pTradeSpi;
		std::map<std::string,std::string> m_ConfigMap;
		CTP_TD_CODE			m_RuningState;
	private:
		std::string		m_BrokerID;
		std::string		m_UserID;
		std::string		m_Password;

		int				m_FrontID;
		int				m_SessionID;
		unsigned int				m_MaxOrderRef;
	};
}


