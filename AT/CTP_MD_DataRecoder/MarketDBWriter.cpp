#include "MarketDBWriter.h"
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>
#include <boost/filesystem.hpp>
#include <sstream>
#include "../HistoryTranslator/SingleDBWriter.h"
MarketDBWriter::MarketDBWriter(const std::vector<std::string>& aList,const std::string & DBpath)
{
	if(!boost::filesystem::exists(DBpath))
	{
		boost::filesystem::create_directory(DBpath);
	}

	for(int i=0;i!= aList.size();++i)
	{
		//leveldb::DB*& lpDB = ;
		//leveldb::Options options;
		//options.create_if_missing = true;
		boost::filesystem::path lDir(DBpath);
		lDir += aList[i];
		std::string lSubItemDBPath = lDir.string();
		m_DBMap[aList[i]].reset(new SingleDBWriter(lSubItemDBPath.c_str()));
	}
}

MarketDBWriter::~MarketDBWriter(void)
{
}

void MarketDBWriter::StroeMarketTick( std::shared_ptr< AT::MarketData> apMarketTick )
{

	std::string lInstrumentID = apMarketTick->InstrumentID;
	m_DBMap[lInstrumentID]->StoreMarketData(apMarketTick);
}

