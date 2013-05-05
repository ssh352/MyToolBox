#pragma  once
#include "MarketData.h"
#include <memory>


namespace AT
{

	class MarketMapWarpper;
	typedef std::shared_ptr<const MarketMapWarpper>  const_MapWarpper_ptr;
	class IMarketCache
	{
		
		//for user
	public:
		
		virtual const_MapWarpper_ptr  GetMarketMapByName(const char* aInstrument)  const = 0;


		//for creator
	public:
		virtual void FeedMarketDepth(std::shared_ptr< MarketData> apMarketDepth) = 0;
	};
}