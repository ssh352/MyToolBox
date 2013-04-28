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
			if(false ==  m_InstrumentMarket.insert(std::make_pair(lItemName,lpMarketMap)).second)
			{
				std::cerr<<"can not add MarkeMap";
				throw std::exception("can not add MarkeMap");
			}

			//LoadFromDB(lpMarketMap.get(),lpDB);

			std::shared_ptr<MarketMapWarpper> lpWarpper(new MarketMapWarpper(lpMarketMap.get(),lItemName) );
			m_AllMarketMap.insert(std::make_pair(lItemName,lpWarpper));
			//leveldb::Iterator* liter = m_pDB->NewIterator(leveldb::ReadOptions());
			//for (liter->SeekToFirst(); liter->Valid(); liter->Next()) 
			//{

			//	memcpy(lStoreInstPtr.get(),liter->value().data(),liter->value().size());
			//	m_ItemMap[liter->key().ToString()] = lStoreInstPtr;
			//}
			//delete liter;

		}





		//using namespace boost::filesystem;
		//path  lDBPath(AconfigFile);
		//for( directory_iterator iter(lDBPath); iter != directory_iterator(); ++iter)
		//{
		//	leveldb::DB* lpDB = nullptr;

		//	leveldb::Options options;
		//	options.create_if_missing = true;
		//	leveldb::Status lstatus = leveldb::DB::Open(options, aDBPath.c_str(), &m_pDB);
		//	if(lstatus.ok())
		//	{
		//		LoadFromDB();
		//	}
		//	else
		//	{
		//		throw std::exception("Open  Db Failed");
		//	}
		//	leveldb::Iterator* liter = m_pDB->NewIterator(leveldb::ReadOptions());
		//	for (liter->SeekToFirst(); liter->Valid(); liter->Next()) 
		//	{

		//		memcpy(lStoreInstPtr.get(),liter->value().data(),liter->value().size());
		//		m_ItemMap[liter->key().ToString()] = lStoreInstPtr;
		//	}
		//	delete liter;


		//	std::shared_ptr< std::map<uint32_t,MarketData> >   lpSotreMap( new std::map<uint32_t,MarketData> );
		//	std::string lInstrumentID ;


		//	bool isDuplicate = ! m_AllMarketMap.insert(make_pair(lInstrumentID,MarketMapWarpper(lpSotreMap.get(),lInstrumentID) ));
		//	if(isDuplicate)
		//	{
		//		std::cerr<<"InsutmentDB is duplicate ignore it";
		//		return ;
		//	}
		//
		//}

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

	 void MarketCache::FeedMarketDepth( const MarketData& aMarketDepth )
	 {
		 std::string  lInstrumentID = aMarketDepth.InstrumentID;
		 uint32_t lKey = aMarketDepth.m_UpdateTime.time_of_day().total_milliseconds();
		 m_InstrumentMarket[lInstrumentID]->insert(std::make_pair(lKey,aMarketDepth));

		 leveldb::Status lStatus = m_InstrumentDBMap[lInstrumentID]->Put(leveldb::WriteOptions(),leveldb::Slice( (char*)&lKey,sizeof(lKey)), leveldb::Slice((char*)&aMarketDepth,sizeof(aMarketDepth)));
		 if(!lStatus.ok())
		 {
			 std::cerr<<"Store  A tick Failed"<< aMarketDepth.ToString();
		 }

	 }

}

