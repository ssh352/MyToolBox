#pragma once
#include <vector>
#include <map>
#include <boost/shared_ptr.hpp>
struct CThostFtdcDepthMarketDataField;

namespace CTP
{
	class MarketDepthCacheCTP
	{
	public:
		typedef 	boost::shared_ptr<CThostFtdcDepthMarketDataField> MarketDataPtr;
		typedef		std::vector<MarketDataPtr>											MarketDataPtrVec;
		MarketDepthCacheCTP(void);
		virtual ~MarketDepthCacheCTP(void);

		void InsertMarketTick( MarketDataPtr  aTickData);

	private:
		std::map<std::string,MarketDataPtrVec>		m_MarketDataMap;	
	};



}


