#pragma once
#include <boost\shared_ptr.hpp>

#include "OrderTable_CTP.h"
#include "TradeTable.h"
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
		std::map<std::string,std::string> m_ThostOrderID;

	};
}


