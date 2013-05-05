#pragma  once
#include "IMarketCache.h"
#include <memory>

namespace leveldb
{
	class DB;
}
namespace AT
{
	class SingleDBHandler;
	class MarketCache :public IMarketCache
	{
		
	public:
		//输入为数据保存地址 遍历
		MarketCache(const char* StoreDBPath);

		void CreateDBitem(const std::string &lDbPathName,const std::string& lItemName );

		~MarketCache(void);
		virtual const_MapWarpper_ptr  GetMarketMapByName(const char* aInstrument) const  override ;
		virtual MarketCacheAllInstruments GetAllInstruments() const override; 

		virtual void FeedMarketDepth(std::shared_ptr< MarketData> apMarketDepth) override;
	

	private:
	
		//这里有点乱 
		//MarketMap是实际保存的map 由m_InstrumentMarket 持有所有权
		//m_AllMarketMap 保存MarketMapWarpper（里面对map进行包装）但是不持有所有权
		//m_InstrumentDBMap 则是进行保存操作的数据库

		std::map<std::string, const_MapWarpper_ptr > m_AllMarketMap;//for all Instrument
		std::map<std::string,std::shared_ptr< MarketMap> > m_InstrumentMarket; // for update 
		std::map<std::string ,std::auto_ptr<SingleDBHandler>> m_InstrumentDBMap;
		std::string m_DBPath;

	};
}

