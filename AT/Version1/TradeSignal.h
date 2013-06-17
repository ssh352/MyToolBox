#pragma  once
#include "AT_Struct.h"
#include <boost\cstdint.hpp>
#pragma  pack(push)
#pragma  pack(8)
namespace AT
{
	using boost::int32_t ;
	struct  TradeSignal
	{
		bool	m_Valid;
		int32_t m_TradeSignalType;
		int32_t m_TradeSignalSequence;
		MarketData	m_TriggerMarketData;
	};
}

#pragma  pack(pop)