#include "MarketMapWarpper.h"


namespace AT
{


	const MarketData& MarketMapWarpper::operator[]( const uint32_t& k ) const
	{
		return at(k);
	}

	MarketMapWarpper::MarketMapWarpper( MarketMap* aMap , std::string aID )
		: m_pMap(aMap)
		, m_InstrumentID(aID)
	{

	}

	MarketMapWarpper::const_iterator MarketMapWarpper::begin() const
	{
		return m_pMap->begin();
	}

	MarketMapWarpper::const_iterator MarketMapWarpper::end() const
	{
		return m_pMap->end();
	}

	const MarketData& MarketMapWarpper::at( const uint32_t& k ) const
	{
		return *m_pMap->at(k);
	}

}
