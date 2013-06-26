#pragma  once
#include "AT_Struct.h"
#include <boost\cstdint.hpp>
#pragma  pack(push)
#pragma  pack(8)
namespace AT
{
	using boost::int32_t ;
	const int g_TradeSignalIDLength = 16;
	struct  TradeSignal
	{
		bool	m_Valid;
		bool	m_BuyOrSell;
		char	m_ID[g_TradeSignalIDLength];
		int32_t m_TradeSignalType;
		int32_t m_TradeSignalSequence;
		int32_t m_priority;
		MarketData	m_TriggerMarketData;
	};
}

#pragma  pack(pop)