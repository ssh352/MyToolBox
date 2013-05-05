#include "SingleDBWriter.h"
#include "MarketData.h"
#include "leveldb/db.h"
#include "ATLogger.h"

namespace AT
{


SingleDBHandler::SingleDBHandler( const char* aStoreFile )
{
	leveldb::Options options;
	options.create_if_missing = true;
	leveldb::Status lstatus = leveldb::DB::Open(options, aStoreFile, &m_pDB);
	if(!lstatus.ok())
	{
		std::string lErrorInfo = "Can not open DB";
		lErrorInfo += aStoreFile;
		ATLOG(AT::LogLevel::L_ERROR,lErrorInfo);
		throw std::logic_error("Can not open DB");
	}

}

SingleDBHandler::~SingleDBHandler()
{
		delete m_pDB;
}

void SingleDBHandler::StoreMarketData(std::shared_ptr<AT::MarketData> aDataPtr)
{
	uint64_t lKey = aDataPtr->m_UpdateTime.time_of_day().total_milliseconds();
	leveldb::Slice lKeySlice ((char*)&lKey,sizeof(lKey));
	leveldb::Slice lValSlice ((char*)aDataPtr.get(),sizeof(AT::MarketData));

    if(! m_pDB->Put(leveldb::WriteOptions(),lKeySlice,lValSlice).ok())
	{
		std::string lErrorInfo = "Store Market Failed";
		lErrorInfo += aDataPtr->ToString();
		ATLOG(AT::LogLevel::L_ERROR,lErrorInfo);
	}

}

void SingleDBHandler::RestoreMarketMap( std::shared_ptr< MarketMap> lpMarketMap  )
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