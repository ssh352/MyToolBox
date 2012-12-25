#include "DataCacheCTP.h"
#include <boost\foreach.hpp>
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

}


