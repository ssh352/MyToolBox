#pragma  once
#include "IMarketCache.h"
namespace AT
{
	class MarketCache :public IMarketCache
	{
	public:
		MarketCache(const char* AconfigFile);
		~MarketCache(void);
	};
}

