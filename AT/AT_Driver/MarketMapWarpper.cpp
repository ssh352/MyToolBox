#include "MarketMapWarpper.h"


namespace AT
{


	const MarketData& MarketMapWarpper::operator[]( const uint32_t k ) const
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
		return const_iterator(m_pMap->begin());
	}

	MarketMapWarpper::const_iterator MarketMapWarpper::end() const
	{
		return  const_iterator(m_pMap->end());
	}

	const MarketData& MarketMapWarpper::at( const uint32_t k ) const
	{
		return *m_pMap->at(k);
	}

	MarketMapWarpper::~MarketMapWarpper( void )
	{

	}

	MarketMapWarpper::const_iterator MarketMapWarpper::lower_bound( const uint32_t k ) const
	{
		return const_iterator(m_pMap->lower_bound(k));
	}

	MarketMapWarpper::const_iterator MarketMapWarpper::upper_bound( const uint32_t k ) const
	{
			return const_iterator(m_pMap->upper_bound(k));
	}


}
