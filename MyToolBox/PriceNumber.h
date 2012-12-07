#pragma once

#include<boost\cstdint.hpp>
#include <boost\operators.hpp>
#include <boost\integer_traits.hpp>
#include <exception>
namespace MY_TOOL_BOX
{
	class PriceNumber : boost::ordered_field_operators<PriceNumber,
						boost::ordered_field_operators<PriceNumber,int,
						boost::ordered_field_operators<PriceNumber,double> > > 
	{
	public:

		PriceNumber(void);
		~PriceNumber(void);

		PriceNumber(const PriceNumber& aRef);

		explicit PriceNumber(int32_t abase,int8_t aExp = 0, int8_t a_Power = 10);
		explicit PriceNumber(double aVal)	throw (std::runtime_error);

		operator int32_t() const;
		operator double() const;

	public:
		//total order
		bool operator<(const PriceNumber& aright);
		bool operator<(const double& aright);
		bool operator>(const double& aright);
		bool operator<(const int& aright);
		bool operator>(const int& aright);
		bool operator==(const PriceNumber& aright);
		bool operator==(const float& aright);
		bool operator==(const int& aright);

		//
		PriceNumber& operator+=(const PriceNumber& aright);
		PriceNumber& operator-=(const PriceNumber& aright);
		PriceNumber& operator*=(const PriceNumber& aright);
		PriceNumber& operator/=(const PriceNumber& aright);

		PriceNumber& operator+=(const double& aright);
		PriceNumber& operator-=(const double& aright);
		PriceNumber& operator*=(const double& aright);
		PriceNumber& operator/=(const double& aright);

		PriceNumber& operator+=(const int& aright);
		PriceNumber& operator-=(const int& aright);
		PriceNumber& operator*=(const int& aright);
		PriceNumber& operator/=(const int& aright);

	private:
		int32_t m_PriceBase;
		int8_t	m_PriceExp;
		int8_t	m_PricePower;

		static const int32_t __MAX_VAL_BEFORE_SHIFT_10 = boost::integer_traits<int32_t>::const_max / 10;

	};
}



