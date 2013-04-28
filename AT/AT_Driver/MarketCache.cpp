#include "MarketCache.h"

namespace  AT
{


	MarketCache::MarketCache( const char* AconfigFile )
	{

	}


	MarketCache::~MarketCache(void)
	{
	}

	 IMarketCache::const_MapWarpper_ptr MarketCache::GetMarketMapByName(const char* aInstrument) const  
	{
		return m_MarketMap.at(std::string(aInstrument));
	}

}

