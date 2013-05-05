#include "AT_OrderTable.h"


namespace AT
{	

	std::shared_ptr<AT::OrderUpdate> AT_OrderTable::FindOrderByPlatformOrderID( const std::string& aPlatformORderID )
	{
		for(auto lPair:m_ItemMap)
		{
			if( aPlatformORderID == lPair.second->m_PlatformOrderID) return lPair.second;
		}

		std::string logStr("Did Not Find Order By aPlatformORderID:");
		logStr += aPlatformORderID;
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

	std::shared_ptr<AT::OrderUpdate> AT_OrderTable::FindOrderByExchangeOrderID( const std::string& aExchangORderID )
	{
		for(auto lPair:m_ItemMap)
		{
			if( aExchangORderID == lPair.second->m_ExchangeOrderID) return lPair.second;
		}

		std::string logStr("Did Not Find Order By ExchangOrderID:");
		logStr += aExchangORderID;
		ATLOG(LogLevel::L_ERROR,logStr);
		return std::shared_ptr<AT::OrderUpdate>();
	}


}

