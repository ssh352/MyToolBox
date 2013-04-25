#pragma once

#include <leveldb/db.h>

#include <string>
#include <map>
#include <vector>
#include "AT_Struct.h"
class MarketDBWriter
{
public:
	MarketDBWriter(const std::vector<std::string>& aList,const std::string & DBpath);
	~MarketDBWriter(void);

	void StroeMarketTick(const AT::MarketData& aMarketTick);
	
private:
	std::map<std::string,leveldb::DB*>			m_DBMap;;

};

