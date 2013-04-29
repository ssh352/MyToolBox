#include "SingleDBWriter.h"
#include "MarketData.h"
#include "leveldb/db.h"

SingleDBWriter::SingleDBWriter( const char* aStoreFile )
{
	leveldb::Options options;
	options.create_if_missing = true;
	leveldb::Status lstatus = leveldb::DB::Open(options, aStoreFile, &m_pDB);
	if(!lstatus.ok())
	{
		throw std::logic_error("Can not create DB");
	}

}

SingleDBWriter::~SingleDBWriter()
{
		delete m_pDB;
}

void SingleDBWriter::StoreMarketData(std::shared_ptr<AT::MarketData> aDataPtr)
{
	uint64_t lKey = aDataPtr->m_UpdateTime.time_of_day().total_milliseconds();
	leveldb::Slice lKeySlice ((char*)&lKey,sizeof(lKey));
	leveldb::Slice lValSlice ((char*)aDataPtr.get(),sizeof(AT::MarketData));

    if(! m_pDB->Put(leveldb::WriteOptions(),lKeySlice,lValSlice).ok())
	{
		std::cerr<<"Store Market Failed"<<aDataPtr->ToString();
	}

}
