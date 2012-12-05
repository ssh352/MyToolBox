#include "PriceNumber.h"

namespace MY_TOOL_BOX
{
	PriceNumber::PriceNumber(void)
		: m_PriceBase(0),
		m_PriceExp(0),
		m_PricePower(10)
	{
	}

	PriceNumber::PriceNumber( const PriceNumber& aRef )
	{
		m_PriceBase = aRef.m_PriceBase;
		m_PriceExp = aRef.m_PriceExp;
		m_PricePower = aRef.m_PricePower;
	}

	PriceNumber::PriceNumber( int32_t abase,int8_t aExp /*= 0*/, int8_t a_Power /*= 10*/ )
	{
		m_PriceBase = abase;
		m_PriceExp = aExp;
		m_PricePower = a_Power;

	}


	PriceNumber::PriceNumber( double aVal )
	{
		m_PriceBase = 0;
		m_PriceExp = 0;
		m_PricePower = 10;

		const double MinValDetected = 0.000001;
		m_PriceBase = static_cast<int32_t>(aVal);
		double lDoublePart = aVal - m_PriceBase;

		while( abs(lDoublePart) > MinValDetected)
		{
			m_PriceBase *= 10;
			m_PriceExp -= 1;
			int32_t lInt   = int32_t( (lDoublePart *10) );
			m_PriceBase += lInt;
			lDoublePart = lDoublePart *10 - lInt;

		}

		if(abs(aVal - double(*this)) > MinValDetected)
		{
			throw std::runtime_error("PriceNumber Convert From double Error");
		}
	}

	PriceNumber::operator int32_t() const 
	{
		return static_cast<int32_t>(double(*this));
	}

	PriceNumber::operator double() const 
	{
		return m_PriceBase* pow(double(m_PricePower),m_PriceExp);
	}

	PriceNumber::~PriceNumber(void)
	{

	}

}


