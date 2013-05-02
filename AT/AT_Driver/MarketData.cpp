#include "MarketData.h"
#include <boost\format.hpp>
namespace AT
{
	std::string MarketData::ToString() const
	{	
		return str(boost::format("%s %s [Buy:%d %u] [Sell:%d %u] [Last:%d]") % InstrumentID  % AT::ToString(m_UpdateTime)
			% m_BidPrice %m_BidVol % m_AskPrice %m_AskVol %m_LastPrice );
	}

}


