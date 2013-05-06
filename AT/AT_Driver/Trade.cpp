#include "Trade.h"
#include <boost\format.hpp>
namespace AT
{
	std::string TradeUpdate::ToString() const
	{
		char	m_TradeID[cTradeIDLength];
		AT_Time m_TradeTime;
		return str(boost::format("TradeUpdate [OrderKey: %s] \n[InstrumentID:: %s] [AccoutID:: %s] \n[Price: %d]  [Vol: %u]\
								 [Time %s] [TradeID: %s]") 
								 % AT::ToString(m_Key)
								 % InstrumentID % AccoutID 
								 % m_TradePrice %m_TradeVol
								 %  AT::ToString(m_TradeTime) 
								 %  m_TradeID
								 );
	}

}

