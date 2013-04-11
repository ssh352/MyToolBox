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

void MarketDBWriter::StroeMarketTick( const std::string& aMarketTick )
{
	std::stringstream lbuf(aMarketTick);
	using boost::property_tree::ptree;
	ptree pt;
	read_xml(lbuf,pt);
	std::string lInstrumentID = pt.get<std::string>("market.ID");
	std::string lTImeSec = pt.get<std::string>("market.Second");
	std::string lMillSec = pt.get<std::string>("market.Millsecond");
	std::string lKey = lTImeSec + '.'+ lMillSec;
	m_DBMap[lInstrumentID]->Put(leveldb::WriteOptions(), lKey, leveldb::Slice(aMarketTick.c_str()));
	std::cout<<aMarketTick<<'\n';
}

