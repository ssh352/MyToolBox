#pragma  once
#include "ConstVarDefine.h"
#pragma  pack(push , 8)
namespace AT
{
	struct TradeUpdate
	{
		char  InstrumentID[cInstrimentIDLength];
		char AccoutID[cAccoutIDLength];
		char	m_TradeID[cTradeIDLength];
		int32_t m_TradePrice;
		uint32_t m_TradeVol;
		AT_Time m_TradeTime;
		AT_Order_Key m_Key;
		BuySellType m_BuySellType; 
		OpenCloseType m_OpenCloseType;
		
	};
	std::string ToString(const TradeUpdate& aTrade) ;
}
#pragma  pack(pop)