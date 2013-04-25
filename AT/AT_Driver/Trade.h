#pragma  once
#include "ConstVarDefine.h"
#pragma  pack(push , 4) //4×Ö½Ú¶ÔÆë
namespace AT
{
	struct Trade
	{
		char  InstrumentID[cInstrimentIDLength];
		char AccoutID[cAccoutIDLength];
		char	m_TradeID[cTradeIDLength];
		int32_t m_TradePrice;
		uint32_t m_TradeVol;
		AT_Time m_TradeTime;
		AT_Order_Key m_OrderKey;	
	};
}
#pragma  pack(pop)