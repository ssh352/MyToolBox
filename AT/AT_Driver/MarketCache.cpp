#include "MarketCache.h"
#include "MarketMapWarpper.h"

#include <boost\property_tree\ptree.hpp>
#include <boost\property_tree\xml_parser.hpp>
//#include <boost\filesystem.hpp>
#include <leveldb\db.h>

#include <iostream>
namespace  AT
{


	MarketCache::MarketCache( const char* AconfigFile )
	{

		boost::property_tree::ptree lConfig;
		read_xml(AconfigFile,lConfig);

		for( std::pair<std::string,boost::property_tree::ptree>  lMarketItemList : lConfig.get_child("MarketCache.Items"))
		{
			std::string lItemName = lMarketItemList.first;
			std::string lItemDBPath = lMarketItemList.second.get<std::string>("Path");

			leveldb::DB* lpDB = nullptr;
			leveldb::Options options;
			options.create_if_missing = true;
			leveldb::Status lstatus = leveldb::DB::Open(options, lItemDBPath.c_str(), &lpDB);

			if(!lstatus.ok())
			{
				std::cerr<< "can not open db "<< lItemDBPath;
				throw std::exception("can not open db");
			}
			m_InstrumentDBMap.insert(std::make_pair(lItemName,lpDB));


			std::shared_ptr<MarketMap> lpMarketMap(new MarketMap);
			RestoreMarketMap(lpMarketMap,lpDB);

			if(false ==  m_InstrumentMarket.insert(std::make_pair(lItemName,lpMarketMap)).second)
			{
				std::cerr<<"can not add MarkeMap";
				throw std::exception("can not add MarkeMap");
			}

			std::shared_ptr<MarketMapWarpper> lpWarpper(new MarketMapWarpper(lpMarketMap.get(),lItemName) );
			m_AllMarketMap.insert(std::make_pair(lItemName,lpWarpper));
		}
	}
	MarketCache::~MarketCache(void)
	{
	}

	 IMarketCache::const_MapWarpper_ptr MarketCache::GetMarketMapByName(const char* aInstrument) const  
	{
		if(m_AllMarketMap.find( std::string(aInstrument)) != m_AllMarketMap.end())
		{
			return m_AllMarketMap.at(std::string(aInstrument));
		}
		else
		{
			throw std::exception("Can not Get Instrument From MarketDataCache");
		}
	}

	 void MarketCache::FeedMarketDepth(std::shared_ptr< MarketData> apMarketDepth)
	 {
		 std::string  lInstrumentID = apMarketDepth->InstrumentID;
		 uint64_t lKey = apMarketDepth->m_UpdateTime.time_of_day().total_milliseconds();
		 m_InstrumentMarket[lInstrumentID]->insert(std::make_pair(lKey,apMarketDepth));

		 leveldb::Status lStatus = m_InstrumentDBMap[lInstrumentID]->Put(leveldb::WriteOptions(),leveldb::Slice( (char*)&lKey,sizeof(lKey)), leveldb::Slice((char*)apMarketDepth.get(),sizeof(MarketData)));
		 if(!lStatus.ok())
		 {
			 std::cerr<<"Store  A tick Failed"<< apMarketDepth->ToString();
		 }

	 }

	 void MarketCache::RestoreMarketMap( std::shared_ptr< MarketMap> lpMarketMap , leveldb::DB* lpDB )
	 {
		 leveldb::Iterator* liter = lpDB->NewIterator(leveldb::ReadOptions());
		 for (liter->SeekToFirst(); liter->Valid(); liter->Next())
		 {
			std::shared_ptr<MarketData> lpMarketData(new MarketData);
			 memcpy(lpMarketData.get(),liter->value().data(),liter->value().size());
			 uint64_t lKey =  0;
			 memcpy(&lKey,liter->key().data(),liter->value().size());
			 lpMarketMap->insert(std::make_pair(lKey,lpMarketData));
		 }
		 delete liter;
	 }

}

