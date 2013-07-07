#include "Trade.h"
#include <boost\format.hpp>
namespace AT
{

	std::string ToString( const TradeUpdate& aTrade )
	{
		return str(boost::format("TradeUpdate [OrderKey: %s] \n[InstrumentID:: %s] [AccoutID:: %s] \n[Price: %d]  [Vol: %u]\
								 [Time %s] [TradeID: %s]") 
								 % AT::ToString(aTrade.m_Key)
								 %  aTrade.InstrumentID % aTrade.AccoutID 
								 %  aTrade.m_TradePrice % aTrade.m_TradeVol
								 %  AT::ToString( aTrade.m_TradeTime) 
								 %  aTrade. m_TradeID
								 );
	}

}

