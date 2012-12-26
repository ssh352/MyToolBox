#include "MarketDepthCacheCTP.h"
#include <string>
#include "CTP_API.h"
#include <boost\lexical_cast.hpp>
namespace CTP
{
	MarketDepthCacheCTP::MarketDepthCacheCTP(void)
	{
	}


	MarketDepthCacheCTP::~MarketDepthCacheCTP(void)
	{
	}

	void MarketDepthCacheCTP::InsertMarketTick( MarketDataPtr aTickData )
	{
		m_MarketDataMap[GenerateTickKey(aTickData)] = aTickData;
	}

	std::string MarketDepthCacheCTP::GenerateTickKey( MarketDataPtr aTickData )
	{
		std::string lInstrumentIDKey (aTickData->InstrumentID,30);
		std::string	lTime(aTickData->UpdateTime);
		//std::string	lTime(boost::lexical_cast<std::string>(aTickData->UpdateMillisec));
		lInstrumentIDKey += lTime;
		return lInstrumentIDKey ;
	}

}

