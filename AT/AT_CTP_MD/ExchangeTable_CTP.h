#pragma once
#include <map>
#include <boost/shared_ptr.hpp>
#include "ItemTable.h"
#include "CTP_API.h"
namespace CTP
{
	typedef boost::shared_ptr<CThostFtdcExchangeField> Exchange_Ptr;

	class CThostFtdcExchangeFieldTraits
	{
	public:
		static std::string GetItemID(Exchange_Ptr apExchange){return std::string(apExchange->ExchangeID,30);};
		static std::string GetItemTypeName(){return "CThostFtdcExchange";};
	};


	class ExchangeTable_CTP :public ItemTable<CThostFtdcExchangeField,CThostFtdcExchangeFieldTraits>
	{
	public:
		void AddExchange(Exchange_Ptr apExchange );
		Exchange_Ptr GetExchange(const std::string& aExchangeID);
	};
}



