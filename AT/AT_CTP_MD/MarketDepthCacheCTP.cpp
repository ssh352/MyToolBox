#include "MarketDepthCacheCTP.h"
#include <string>
#include "CTP_API.h"
#include <boost\lexical_cast.hpp>
#include "leveldb/db.h"
#include <iostream>
namespace CTP
{
	void MarketDepthCacheCTP::InsertMarketTick( MarketDataPtr aTickData )
	{
		AddItem(aTickData);
	}
}

