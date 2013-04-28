#pragma  once
#include "MarketData.h"
#include <memory>


namespace AT
{

	class MapWarpper;
	class IMarketCache
	{
	public:

		virtual std::shared_ptr<MapWarpper>  GetMarketMapByName(const char* aInstrument)  const = 0;

	};
}