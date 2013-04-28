#pragma  once
#include "MarketData.h"
#include <memory>


namespace AT
{

	class MarketMapWarpper;
	class IMarketCache
	{
		
		//for user
	public:
		typedef std::shared_ptr<const MarketMapWarpper>  const_MapWarpper_ptr;
		virtual const_MapWarpper_ptr  GetMarketMapByName(const char* aInstrument)  const = 0;


		//for creater
	public:
		virtual void FeedMarketDepth(const MarketData& aMarketDepth) = 0;
	};
}