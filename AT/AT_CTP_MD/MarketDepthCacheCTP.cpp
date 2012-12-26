#include "MarketDepthCacheCTP.h"
#include <string>
#include "CTP_API.h"
#include <boost\lexical_cast.hpp>
#include "leveldb/db.h"
#include <iostream>
namespace CTP
{
	MarketDepthCacheCTP::MarketDepthCacheCTP(void)
	{
	}


	MarketDepthCacheCTP::~MarketDepthCacheCTP(void)
	{
		delete m_pDB;
	}

	void MarketDepthCacheCTP::InsertMarketTick( MarketDataPtr aTickData )
	{
		std::string lTickKey = GenerateTickKey(aTickData);
		m_MarketDataMap[lTickKey] = aTickData;


		leveldb::Status lstatus  =m_pDB->Put(leveldb::WriteOptions(), lTickKey, leveldb::Slice(((char*)aTickData.get()), sizeof(CThostFtdcDepthMarketDataField)));
		if(!lstatus.ok())
		{
			std::cerr<<"Store MarketTick failed"<<std::endl;
		}
	}

	std::string MarketDepthCacheCTP::GenerateTickKey( MarketDataPtr aTickData )
	{
		std::string lInstrumentIDKey (aTickData->InstrumentID,30);
		std::string	lTime(aTickData->UpdateTime);
		//std::string	lTime(boost::lexical_cast<std::string>(aTickData->UpdateMillisec));
		lInstrumentIDKey += lTime;
		return lInstrumentIDKey ;
	}

	void MarketDepthCacheCTP::InitWithDB( const std::string& aDBPath )
	{

		if(aDBPath.empty())
		{
			throw std::exception("Do not Set MarketDepthCacheCtp Db path");
		}

		leveldb::Options options;
		options.create_if_missing = true;
		leveldb::Status lstatus = leveldb::DB::Open(options, aDBPath.c_str(), &m_pDB);
		if(lstatus.ok())
		{
			LoadFromDB();
		}
		else
		{
			throw std::exception("Open  MarketDepthCacheCtp Db Failed");
		}

	}

	void MarketDepthCacheCTP::LoadFromDB()
	{
		leveldb::Iterator* liter = m_pDB->NewIterator(leveldb::ReadOptions());
		for (liter->SeekToFirst(); liter->Valid(); liter->Next()) 
		{
			MarketDataPtr lTickPtr(new CThostFtdcDepthMarketDataField);
			memcpy(lTickPtr.get(),liter->value().data(),liter->value().size());
			m_MarketDataMap[liter->key().ToString()] = lTickPtr;
		}
		delete liter;
	}

}

