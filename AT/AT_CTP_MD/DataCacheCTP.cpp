#include "DataCacheCTP.h"
#include "MarketDepthCacheCTP.h"
#include <boost\date_time.hpp>
#include <boost/date_time/gregorian/gregorian.hpp>
namespace CTP
{
	void DataCacheCTP::AddExchange( Exchange_Ptr apExchange )
	{
		m_ExchangeMap.AddExchange(apExchange);
	}

	void DataCacheCTP::AddInstrument( Instrument_Ptr apInstrument )
	{
		m_InstrumentMap.AddInstrument(apInstrument);
	}

	std::vector<std::string> DataCacheCTP::GetInstrumentListByExchange( const std::string& ExchangID )
	{
		return GetInstrumentListByPropert<FindByExchangeID>(ExchangID);
	}

	std::vector<std::string> DataCacheCTP::GetInstrumentListByProduct( const std::string& ProductID )
	{
		return GetInstrumentListByPropert<FindByProductID>(ProductID);
	}

	std::vector<std::string> DataCacheCTP::GetInstrumentListAll()
	{
		return GetInstrumentListByPropert<FindAllInstrument>(std::string());
	}

	std::set<std::string> DataCacheCTP::GetProductList()
	{
		std::set<std::string> lRet;

		for(InstrumentTable_CTP::iterator iter = m_InstrumentMap.begin();iter != m_InstrumentMap.end();iter++)
		{
			lRet.insert(std::string(iter->second->ProductID));
		}
		return lRet;
	}

	void DataCacheCTP::AddMarketTick( MarketDataPtr aTick )
	{
		m_MarketDepthCacheCTP.InsertMarketTick(aTick);
	}

	DataCacheCTP::DataCacheCTP()
	{
		std::string lDateString= boost::gregorian::to_iso_string( boost::gregorian::day_clock::local_day());
		m_InstrumentMap.InitWithDB("InstrumentMapDB");
		m_ExchangeMap.InitWithDB("ExchangeMapDB");
		m_MarketDepthCacheCTP.InitWithDB(lDateString+"MarkeDepthDB");
	}

	MarketDepthCacheCTP* DataCacheCTP::GetMarketDepthCache()
	{
		return &m_MarketDepthCacheCTP;
	}

}


