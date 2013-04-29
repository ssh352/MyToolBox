#ifndef MARKETDATATRANSLATOR_H_INCLUDED
#define MARKETDATATRANSLATOR_H_INCLUDED

#include <string>
namespace AT
{
    class MarketData;
}
class SingleDBwriter;

class MarketDataTransLator
{
public:
    MarketDataTransLator(const std::string& aDBstoreDIr);
    virtual ~MarketDataTransLator(void);


    void PraseDir(const std::string& aDirName);
    void PraseFile(const std::string& aFileName);
    void PraseLine(const std::string& aLIne);
    AT::AT_TIME PraseTime(const std::string aDate, const std::string& aTime);

private:
    std::auto_ptr<SingleDBwriter>   m_pDBWriter;
    std::string                     m_DBFoudler;

};

#endif // MARKETDATATRANSLATOR_H_INCLUDED

