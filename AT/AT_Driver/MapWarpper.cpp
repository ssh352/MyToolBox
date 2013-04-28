#include "MapWarpper.h"


namespace AT
{


	const MarketData& MapWarpper::operator[]( const uint32_t& k )
	{
		return m_pMap->at(k);
	}

	MapWarpper::MapWarpper( MarketMapType* aMap , std::string aID )
		: m_pMap(aMap)
		, m_InstrumentID(aID)
	{

	}

	MapWarpper::iterator MapWarpper::begin()
	{
		return m_pMap->begin();
	}

	MapWarpper::iterator MapWarpper::end()
	{
		return m_pMap->end();
	}

}
