#include "MarketDBWriter.h"
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>
#include <boost/filesystem.hpp>
#include <sstream>

MarketDBWriter::MarketDBWriter(const std::vector<std::string>& aList,const std::string & DBpath)
{
	if(!boost::filesystem::exists(DBpath))
	{
		boost::filesystem::create_directory(DBpath);
	}

	for(int i=0;i!= aList.size();++i)
	{
		leveldb::DB*& lpDB =  m_DBMap[aList[i]];
		leveldb::Options options;
		options.create_if_missing = true;
		boost::filesystem::path lDir(DBpath);
		lDir += aList[i];
		std::string lSubItemDBPath = lDir.string();
		leveldb::Status lstatus = leveldb::DB::Open(options, lSubItemDBPath.c_str(), &lpDB);
		if(!lstatus.ok())
		{
			std::cerr<<"Open DB failed";
		}
	}
}

MarketDBWriter::~MarketDBWriter(void)
{
}

void MarketDBWriter::StroeMarketTick( const AT::MarketData& aMarketTick )
{
	std::string lInstrumentID = aMarketTick.InstrumentID;
	uint32_t lKey = aMarketTick.m_UpdateTime.time_of_day().total_milliseconds();
	m_DBMap[lInstrumentID]->Put(leveldb::WriteOptions(),leveldb::Slice( (char*)&lKey,sizeof(lKey)), leveldb::Slice((char*)&aMarketTick,sizeof(aMarketTick)));
}

