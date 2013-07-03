#include "SingleDBWriter.h"
#include "MarketData.h"
#include "leveldb/db.h"
#include "ATLogger.h"
#include "ConstVarDefine.h"

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
		memcpy(&lKey,liter->key().data(),liter->key().size());
		lpMarketMap->insert(std::make_pair(lKey,lpMarketData));
	}
	delete liter;
}

void SingleDBHandler::StoreTradeVolData(std::shared_ptr<AT::TradeVolData> aTradeVol)
{
	leveldb::Slice lKeySlice = boost::gregorian::to_iso_string(AT::AT_Local_Time().date());
	leveldb::Slice lValSlice ((char*)aTradeVol.get(),sizeof(AT::TradeVolData));

	if(! m_pDB->Put(leveldb::WriteOptions(),lKeySlice,lValSlice).ok())
	{
		std::string lErrorInfo = "Store TradeVol Failed";
		lErrorInfo += aTradeVol->ToString();
		ATLOG(AT::LogLevel::L_ERROR,lErrorInfo);
	}
}
void SingleDBHandler::RestoreTradeVolData(std::shared_ptr<AT::TradeVolData> aTradeVol)
{
	leveldb::Slice lKeySlice = boost::gregorian::to_iso_string(AT::AT_Local_Time().date());
	std::string KeyValue;
	leveldb::Status s = m_pDB->Get(leveldb::ReadOptions(),lKeySlice,&KeyValue);
	if( !s.ok() )
	{
		std::string lErrorInfo = "ReStore TradeVol Failed";
		ATLOG(AT::LogLevel::L_ERROR,lErrorInfo);
		aTradeVol->m_AutoTradeTime = 0;
		aTradeVol->m_BuyDirectionVol = 0;
		aTradeVol->m_SellDirectionVol = 0;
		aTradeVol->m_TotalCancleTime = 0;
		aTradeVol->m_TotalOpenVol = 0;
		return;
	}

	memcpy(aTradeVol.get(),KeyValue.data(),KeyValue.size());

}

}