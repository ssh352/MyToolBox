#ifndef SINGLEDBWRITER_H
#define SINGLEDBWRITER_H

#include <memory>
#include "MarketData.h"
#include "TradeVol.h"

namespace leveldb
{
	class DB;
}

namespace AT
{
	struct  MarketData;
	class SingleDBHandler
	{
	public:
		SingleDBHandler(const char* aStoreFile);
		virtual ~SingleDBHandler();

		void StoreMarketData(std::shared_ptr<AT::MarketData> aDatPtr);
		void RestoreMarketMap( std::shared_ptr< MarketMap> lpMarketMap  );
		//存储交易量数据
		void StoreTradeVolData(std::shared_ptr<AT::TradeVolData> aTradeVol);
		//读取交易量数据
		void RestoreTradeVolData(std::shared_ptr<TradeVolMap> lpTradeVolMap);

	private:

		leveldb::DB*  m_pDB;
	};
}


#endif // SINGLEDBWRITER_H
