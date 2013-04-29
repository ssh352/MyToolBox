#ifndef SINGLEDBWRITER_H
#define SINGLEDBWRITER_H

#include <memory>
namespace AT
{
    class MarketData;
}
class SingleDBWriter
{
    public:
        SingleDBWriter();
        virtual ~SingleDBWriter();

        void StoreMarketData(std::shared_ptr<AT::MarketData> aDatPtr);
    protected:
    private:

        leveldb::DB*  m_pDB;
};

#endif // SINGLEDBWRITER_H
