#include "MarketCache.h"
#include "MarketMapWarpper.h"
#include "SingleDBWriter.h"
#include "ATLogger.h"
#include <boost\filesystem.hpp>
#include <iostream>
namespace  AT
{
	using namespace boost::filesystem;

	MarketCache::MarketCache( const char* AconfigFile )
		:m_DBPath(AconfigFile)
	{
		

		path lDir(m_DBPath);
		if(!exists(lDir))
		{
			boost::system::error_code lErr;
			create_directory(lDir,lErr);
			std::cout<<lErr.message();
			return;
		}
		else
		{
			for (directory_iterator iter = directory_iterator(lDir); iter != directory_iterator(); ++iter)
			{

				std::string lItemName = iter->path().filename().string();
				std::string lDbPathName = iter->path().string().c_str();

				CreateDBitem(lDbPathName, lItemName);

			}
		}
	}
	MarketCache::~MarketCache(void)
	{
	}

	const_MapWarpper_ptr MarketCache::GetMarketMapByName(const char* aInstrument) const  
	{
		if(m_AllMarketMap.find( std::string(aInstrument)) != m_AllMarketMap.end())
		{
			return m_AllMarketMap.at(std::string(aInstrument));
		}
		else
		{
			ATLOG(AT::LogLevel::L_ERROR,"Can not Get Instrument From MarketDataCache");
			throw std::exception("Can not Get Instrument From MarketDataCache");
		}
	}


	AT::MarketCacheAllInstruments MarketCache::GetAllInstruments() const 
	{
		MarketCacheAllInstruments lret;
		lret.m_CacheSize = m_AllMarketMap.size();
		int index = 0;
		for(auto lPair:m_AllMarketMap)
		{
			lret.m_CacheList[index++] = lPair.second;
		}
		return std::move(lret);
	}


	 void MarketCache::FeedMarketDepth(std::shared_ptr< MarketData> apMarketDepth)
	 {
		 std::string  lInstrumentID = apMarketDepth->InstrumentID;
		 uint64_t lKey = apMarketDepth->m_UpdateTime.time_of_day().total_milliseconds();

		 if(m_InstrumentMarket.count(lInstrumentID) == 0 )
		 {
			 path lDir(m_DBPath);
			 lDir /= lInstrumentID;
			 CreateDBitem(lDir.string(),lInstrumentID);		
		 }
		 m_InstrumentMarket[lInstrumentID]->insert(std::make_pair(lKey,apMarketDepth));
		 m_InstrumentDBMap[lInstrumentID]->StoreMarketData(apMarketDepth);

	 }

	 void MarketCache::CreateDBitem( const std::string &lDbPathName,const std::string& lItemName  )
	 {
		 std::shared_ptr<SingleDBHandler> lpDB(new SingleDBHandler(lDbPathName.c_str()));

		 m_InstrumentDBMap.insert(std::make_pair(lItemName,lpDB));

		 std::shared_ptr<MarketMap> lpMarketMap(new MarketMap);
		 lpDB->RestoreMarketMap(lpMarketMap);

		 if(false ==  m_InstrumentMarket.insert(std::make_pair(lItemName,lpMarketMap)).second)
		 {
			 ATLOG(AT::LogLevel::L_ERROR,"can not add MarkeMap");
			 throw std::exception("can not add MarkeMap");
		 }
		 std::shared_ptr<MarketMapWarpper> lpWarpper(new MarketMapWarpper(lpMarketMap.get(),lItemName) );
		 m_AllMarketMap.insert(std::make_pair(lItemName,lpWarpper));
	 }

}

