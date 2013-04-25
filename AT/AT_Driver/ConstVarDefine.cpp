#include "ConstVarDefine.h"
namespace AT
{

	extern const AT_Time AT_INVALID_TIME = boost::posix_time::ptime(not_a_date_time);


	AT_Time AT_Local_Time()
	{
		return boost::posix_time::microsec_clock::universal_time();
	}

	AT_Time AT_Local_Time()
	{
		{
			return boost::posix_time::microsec_clock::local_time();
		}
	}

	double TransPriceToDouble( int32_t aATPrice )
	{
		return aATPrice * 0.001;
	}

	int32_t TranPriceToInt( double aExchangePrice )
	{
		return int32_t(aExchangePrice*1000 + 0.1);
	}

}
	

