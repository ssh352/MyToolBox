#pragma  once
#include "AT_Struct.h"
#include <boost\cstdint.hpp>
#pragma  pack(push)
#pragma  pack(8)
namespace AT
{
	using boost::int32_t ;
	const int g_TradeSignalIDLength = 16;
	struct  Signal
	{
		bool	m_Valid;
		char	m_ID[g_TradeSignalIDLength];//信号名
		int32_t m_TradeSignalSequence;
		int32_t m_priority;//优先级
		MarketData	m_TriggerMarketData;
	};
}

#pragma  pack(pop)