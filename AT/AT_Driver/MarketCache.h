#pragma  once
#include "IMarketCache.h"
namespace AT
{
	class ImpMarketCache;
	class MarketCache :public IMarketCache
	{
	public:
		MarketCache(void);
		~MarketCache(void);
	};
}

