#ifndef SINGLEDBWRITER_H
#define SINGLEDBWRITER_H

#include <memory>
#include "MarketData.h"

namespace leveldb
{
	class DB;
}

namespace AT
{
	struct  MarketData;
	class SingleDBWriter
	{
	public:
		SingleDBWriter(const char* aStoreFile);
		virtual ~SingleDBWriter();

		void StoreMarketData(std::shared_ptr<AT::MarketData> aDatPtr);
		void RestoreMarketMap( std::shared_ptr< MarketMap> lpMarketMap  );

	private:

		leveldb::DB*  m_pDB;
	};
}


#endif // SINGLEDBWRITER_H
