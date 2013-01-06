#pragma once

#include <map>
#include <boost/shared_ptr.hpp>
#include "ItemTable.h"
#include "CTP_API.h"
namespace CTP
{
	typedef boost::shared_ptr<CThostFtdcTradeField> Trade_Ptr;

	class CThostFtdcTradeFieldTraits
	{
	public:
		static std::string GetItemID(Trade_Ptr apTrade)
		{
			//GenerateExchangeOrderID(apTrade)
			std::string lTradeID(apTrade->TradeID,30);
			return lTradeID;
		}
		static std::string GetItemTypeName(){return "CThostFtdcTradeField";};
		static const int IsDuplicateException = 0;
	};


	class TradeTable_CTP :public ItemTable<CThostFtdcTradeField,CThostFtdcTradeFieldTraits>
	{
	public:
		void AddTrade(Trade_Ptr apOrder );
		Trade_Ptr GetTrade(const std::string& aTradeID);
	};
}


