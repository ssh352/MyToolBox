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
	//	uint32_t m_LastVol;
		uint32_t m_trunover;
		MarketData()
		{
			memset(InstrumentID,0,cInstrimentIDLength);
			m_UpdateTime = AT_INVALID_TIME;
			m_BidPrice = AT_INVALID_PRICE;
			m_BidVol = AT_INVALID_VOL;
			m_AskPrice = AT_INVALID_PRICE;
			m_AskVol = AT_INVALID_VOL;
			m_LastPrice = AT_INVALID_PRICE;
	//		m_LastVol = AT_INVALID_VOL;
			m_trunover  = AT_INVALID_VOL;
		}
	};

}
