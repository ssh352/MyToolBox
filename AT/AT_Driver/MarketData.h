#pragma once
#include "ConstVarDefine.h"

namespace AT
{

	//Ϊ��Ч�ʣ�ֱ��ʹ�ýṹ�壬���Ա�����dll֮�䴫��string�����string�н������ݵ�����

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
