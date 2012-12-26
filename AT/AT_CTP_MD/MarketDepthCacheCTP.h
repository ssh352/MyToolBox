#pragma once
#include <vector>
#include <map>
#include <boost/shared_ptr.hpp>
struct CThostFtdcDepthMarketDataField;

namespace CTP
{
	typedef 	boost::shared_ptr<CThostFtdcDepthMarketDataField> MarketDataPtr;
	typedef		std::vector<MarketDataPtr>						MarketDataPtrVec;
	class MarketDepthCacheCTP
	{
	public:
		
		MarketDepthCacheCTP(void);
		virtual ~MarketDepthCacheCTP(void);

		void InsertMarketTick( MarketDataPtr  aTickData);

	private:
		std::string GenerateTickKey( MarketDataPtr  aTickData);

	private:
		std::map<std::string,MarketDataPtr>		m_MarketDataMap;
	};



}


