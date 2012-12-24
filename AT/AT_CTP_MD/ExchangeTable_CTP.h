#pragma once
#include <map>
#include <boost/shared_ptr.hpp>
class CThostFtdcExchangeField;
namespace CTP
{
	typedef boost::shared_ptr<CThostFtdcExchangeField> Exchange_Ptr;
	class ExchangeTable_CTP
	{
	public:
		ExchangeTable_CTP(void);
		~ExchangeTable_CTP(void);

	public:
		void AddExchange(Exchange_Ptr apExchange );
		Exchange_Ptr GetExchange(const std::string& aExchangeID);
	private:
		std::map<std::string,Exchange_Ptr > m_ExchangeMap;
	};
}



