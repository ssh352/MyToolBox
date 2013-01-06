#pragma once
#include <boost\shared_ptr.hpp>

struct CThostFtdcOrderField;
struct CThostFtdcTradeField;
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

	};
}


