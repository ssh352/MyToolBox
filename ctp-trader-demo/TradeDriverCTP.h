#pragma once
#include "ITradeDriver.h"
#include "OrderTable_CTP.h"
#include "TradeTable_CTP.h"
#include "CTP_API.h"


struct CThostFtdcInputOrderField;
struct CThostFtdcTradeField;
struct CThostFtdcOrderField;
class IClientOrderIDGenerator;


namespace CTP
{
	class TradeDriverCTP :public ITradeDriver<CThostFtdcInputOrderField,CThostFtdcOrderField,CThostFtdcTradeField>
	{
	public:
		virtual void CreateOrder(boost::shared_ptr<CThostFtdcInputOrderField>  aNewOrderPtr);
		virtual void DeleteOrder(std::string ClientOrderId) ;
		virtual IClientOrderIDGenerator* GetClientOrderIDGeneratorPtr();

	private:
		TradeTable_CTP m_TradeTable;
		OrderTable_CTP m_OrderTable_CTP;
		
	};
}



