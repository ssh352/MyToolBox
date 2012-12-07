#include "PriceNumber.h"

namespace MY_TOOL_BOX
{
const double MinValDetected = 0.000001;
const double Double_Factor = 2 * MinValDetected;

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



	bool PriceNumber::operator<( const PriceNumber& aright )
	{
	
		return *this<double(aright);
	}

	bool PriceNumber::operator<( const double& aright )
	{
		return (double(*this) - aright) < Double_Factor;
	}

	bool PriceNumber::operator>( const double& aright )
	{
		return (double(*this) - aright) > Double_Factor;
	}

	bool PriceNumber::operator>( const int& aright )
	{
		return (double(*this) - aright) >Double_Factor;
	}

	bool PriceNumber::operator<( const int& aright )
	{
		return (double(*this) - aright) < Double_Factor;
	}

	bool PriceNumber::operator==( const PriceNumber& aright )
	{
		if(this == &aright)
			return true;
		if(aright.m_PriceBase == m_PriceBase && aright.m_PriceExp == aright.m_PriceExp && m_PricePower == aright.m_PricePower)
		{
			return true;
		}
		return (*this == double(aright));
	}

	bool PriceNumber::operator==( const float& aright )
	{
		return abs(double(*this) - double(aright) ) < Double_Factor;
	}

}


