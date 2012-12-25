#include "MarketDepthCacheCTP.h"
#include <string>
#include "CTP_API.h"
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
		std::string lInstrumentID (aTickData->InstrumentID,30);
		if(!m_MarketDataMap.count(lInstrumentID))
		{
			MarketDataPtrVec lEmptyVec;
			m_MarketDataMap[lInstrumentID] = lEmptyVec;
		}
		m_MarketDataMap[lInstrumentID].push_back(aTickData);
	}
}

