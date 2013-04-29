#ifndef SINGLEDBWRITER_H
#define SINGLEDBWRITER_H

#include <memory>
namespace AT
{
	struct  MarketData;
}

namespace leveldb
{
	class DB;
}
class SingleDBWriter
{
public:
	SingleDBWriter(const char* aStoreFile);
	virtual ~SingleDBWriter();

	void StoreMarketData(std::shared_ptr<AT::MarketData> aDatPtr);
private:

	leveldb::DB*  m_pDB;
};

#endif // SINGLEDBWRITER_H
