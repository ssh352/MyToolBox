#include "SingleDBWriter.h"
#include "MarketData.h"
SingleDBWriter::SingleDBWriter()
{
    leveldb::read
    m_pDB
    //ctor
    //open db
}

SingleDBWriter::~SingleDBWriter()
{
    //dtor
    //close db
}

void SingleDBWriter::StoreMarketData(std::shared_ptr<AT::MarketData> aDatPtr)
{
    uint64_t lKey = aDataPtr->m_UpdateTime.time_of_day().total_millseconds();
    leveldb::Slice lKeySlice (&lKey,sizeof(lKey));
    leveldb::Slice lValSlice (aDatPtr.get(),sizeof(AT::MarketData));

    m_pDB->put()

}
