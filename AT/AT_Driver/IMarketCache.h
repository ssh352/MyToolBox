#pragma  once
#include "MarketData.h"
#include <memory>



namespace AT
{

#pragma  pack(push , 8)
	class MarketMapWarpper;
	typedef std::shared_ptr<const MarketMapWarpper>  const_MapWarpper_ptr;

	static const int gMaxCacheInstrumentNum = 512;
	struct MarketCacheAllInstruments
	{
		int32_t				 m_CacheSize ;
		const_MapWarpper_ptr m_CacheList[gMaxCacheInstrumentNum];

	};
#pragma  pack(pop)
	class IMarketCache
	{
		
		//for user
	public:
		
	
		virtual const_MapWarpper_ptr  GetMarketMapByName(const char* aInstrument) const = 0;
		virtual MarketCacheAllInstruments GetAllInstruments() const = 0; 


		//for creator
	public:
		virtual void FeedMarketDepth(std::shared_ptr< MarketData> apMarketDepth) = 0;
	};
}
