#include "MapWarpper.h"


namespace AT
{


	const MarketData& MapWarpper::operator[]( const uint32_t& k ) const
	{
		return at(k);
	}

	MapWarpper::MapWarpper( MarketMapType* aMap , std::string aID )
		: m_pMap(aMap)
		, m_InstrumentID(aID)
	{

	}

	MapWarpper::const_iterator MapWarpper::begin() const
	{
		return m_pMap->begin();
	}

	MapWarpper::const_iterator MapWarpper::end() const
	{
		return m_pMap->end();
	}

	const MarketData& MapWarpper::at( const uint32_t& k ) const
	{
		return m_pMap->at(k);
	}

}
