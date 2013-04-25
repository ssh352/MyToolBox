#pragma once
#include "ConstVarDefine.h"

namespace AT
{

	//为了效率，直接使用结构体，可以避免在dll之间传递string，与从string中解析数据的问题

	struct MarketData
	{
		//without null end
		char InstrumentID[cInstrimentIDLength];
		AT_Time m_UpdateTime;
		int32_t m_BidPrice;
		uint32_t m_BidVol;
		int32_t m_AskPrice;
		uint32_t m_AskVol;
		int32_t	m_LastPrice;
		uint32_t m_LastVol;
		uint32_t m_trunover;
	};

}
