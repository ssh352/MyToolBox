

#include <MarketData>
#include <boost\filesystem.hpp>
#include <iostream>
#include <boost\tokenizer.hpp>
#include <boost\date_time\gregorian\gregorian.hpp>
#include <boost\date_time\posix_time\conversion.hpp>

using namespace boost::filesystem;
MarketDataTransLator::MarketDataTransLator(const std::string& aDBstoreDIr)
:m_DBFoudler(aDBstoreDIr)
{

}

MarketDataTransLator::~MarketDataTransLator(void)
{

}

void MarketDataTransLator::PraseDir(const std::string& aDirName)
{
    path lDirPath(aDirName);
    if(!exists(lDirPath) || !is_directory(lDirPath))
    {
        std::cerr<<"Dir is not exist"<<std::endl;
    }

    for(directory_iterator iter=  directory_iterator(lDirPath) ; iter != directory_iterator() ; ++ iter)
    {
        PraseFile(iter->path().string());
    }
}

void MarketDataTransLator::PraseFile(const std::string& aFileName)
{
    std::fstream lfile(aFileName,std::ios_base::in);
    if(! lfile.good())
    {
        std::cerr<<"File "<<aFileName<<" Can not open\n";
        return;
    }

    std::string lDBPath = m_DBFoudler+ aFileName;
   // m_pDBWriter.reset(new SingleDBWriter(lDBPath.c_str()));

    size_t lBuffsize = 256;
    char* lineBuff = new char [lBuffsize];
    while(lfile.getline(lineBuff,lBuffsize))
    {
        PraseFile(std::string(lineBuff,std::min(strlen(lineBuff,lBuffsize))));
    }
}

void MarketDataTransLator::PraseLine(const std::string& aLIne)
{

    try
    {
    boost::char_separator<char> lSep(",");
    boost::tokenizer<boost::char_separator<char> > ltoken(aLIne,lSep);
    std::vector<std::string> lWordVec(ltoken.begin(),ltoken.end());
    if(lWordVec.size() != 8)
    {
            std::cerr<<"Bad Line "<< aLIne<<std::endl;
    }

    AT::MarketData lRetMarket;
    lRetMarket.m_UpdateTime = PraseTime(lWordVec[0],lWordVec[1]);
    lRetMarket.m_LastPrice = AT::TranPriceToInt(std::stod(lWordVec[2]));

    lRetMarket.m_AskPrice = AT::TranPriceToInt(std::stod(lWordVec[3]));
    lRetMarket.m_AskVol = std::stoi(lWordVec[4]);
    lRetMarket.m_BidPrice = AT::TranPriceToInt(std::stod(lWordVec[5));
    lRetMarket.m_BidVol = std::stoi(lWordVec[6]);

    m_pDBWriter->StoreMarketData(lRetMarket);
    }
    cache(...)
    {
        std::cerr<<"Bad Line "<< aLIne<<std::endl;
    }
}
AT::AT_TIME MarketDataTransLator::PraseTime(const std::string aDate, const std::string& aTime)
{
    try
    {
        boost::gregorian::date ldate(boost::gregorian::from_undelimited_string(aDate));
        boost::char_separator<char> lSep(":.");
        boost::tokenizer<boost::char_separator<char>> ltoken(aTime,lSep);
        std::vector<std::string> lWordVec(ltoken.begin(),ltoken.end());
        int hour = std::stoi(lWordVec[0]);
        int min = std::stoi(lWordVec[1]);
        int second = std::stoi(lWordVec[2]);
        int millsecond = std::stoi(lWordVec][3]);
        boost::posix_time::time_duration lTimeDur(hour,min,second,0);
        lTimeDur += boost::posox_time::millseconds(millsecond);
        AT::AT_Time ltime (ldate,lTimeDur);
        return std::move(ltime);
    }
    cache(...)
    {
        std::cerr<<"Bad Date Time "<< aDate<<"  "<< aTime<<'\n';
        throw;
    }
}


