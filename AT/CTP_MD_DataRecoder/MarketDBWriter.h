#pragma once

#include <leveldb/db.h>

#include <string>
#include <map>
#include <vector>
class MarketDBWriter
{
public:
	MarketDBWriter(const std::vector<std::string>& aList,const std::string & DBpath);
	~MarketDBWriter(void);

	void StroeMarketTick(const std::string& aMarketTick);
	
private:
	std::map<std::string,leveldb::DB*>			m_DBMap;;

};

