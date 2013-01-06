#pragma once
#include <boost\shared_ptr.hpp>

#include "OrderTable_CTP.h"
#include "TradeTable_CTP.h"
namespace CTP
{
	class DataCache_CTP_TD
	{
	public:
		DataCache_CTP_TD(void);
		~DataCache_CTP_TD(void);
	public:
		void UpdataOrder(boost::shared_ptr<CThostFtdcOrderField>  apOrder);
		void UpdataTrade(boost::shared_ptr<CThostFtdcTradeField> apTrade);

	private:
		void UpdataOrderIDMap(boost::shared_ptr<CThostFtdcOrderField>  apOrder);
		std::map<std::string,std::string> m_ThostOrderID_ExchangOrderID;
		std::map<std::string,std::string> m_ExchangOrderID_ThostOrderID;
		OrderTable_CTP		m_OrderTable;
		TradeTable_CTP		m_TradeTable;


	};
}


