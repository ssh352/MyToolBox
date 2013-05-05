#include "AT_OrderTable.h"


namespace AT
{	

	std::shared_ptr<AT::OrderUpdate> AT_OrderTable::FindOrderByExchangeOrderID( const std::string& aExchangORderID )
	{
		for(auto lPair:m_ItemMap)
		{
			if( aExchangORderID == lPair.second->ExchangeOrderID) return lPair.second;
		}

		std::string logStr("Did Not Find Order By ExchangOrderID:");
		logStr += aExchangORderID;
		ATLOG(LogLevel::L_ERROR,logStr);
		return std::shared_ptr<AT::OrderUpdate>();

	}

	AT_OrderTable::AT_OrderTable( const char* aOrderTableDBpaht )
		:ItemTable(aOrderTableDBpaht)
	{

	}

	AT_OrderTable::~AT_OrderTable()
	{

	}

}

