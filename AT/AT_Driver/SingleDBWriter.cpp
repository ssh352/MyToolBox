#include "SingleDBWriter.h"
#include "MarketData.h"
#include "leveldb/db.h"


namespace AT
{


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

void SingleDBWriter::RestoreMarketMap( std::shared_ptr< MarketMap> lpMarketMap  )
{
	leveldb::Iterator* liter = m_pDB->NewIterator(leveldb::ReadOptions());
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