#pragma once

#include "ConstVarDefine.h"

namespace AT
{
	struct NewOrder
	{
		//without null end
		char InstrumentID[cInstrimentIDLength];
		char AccoutID[cAccoutIDLength];
		int32_t m_Price;
		uint32_t m_Vol;
		BuySellType m_BuySellType; 
		OrderType m_OrderType;
		OpenCloseType m_OpenCloseType;
		AT_Order_Key m_Key;
		char m_MoreInfo[32];
	};

	struct CancelOrder
	{
		char AccoutID[cAccoutIDLength];
		AT_Order_Key m_Key;
	};

	struct ModifyOrder
	{
		//todo
	};


	struct  OrderUpdate
	{
		char InstrumentID[cInstrimentIDLength];
		char AccoutID[cAccoutIDLength];
		int32_t m_Price;
		uint32_t m_Vol;		//单子的总数量 ，如果通过modify改小 这个数量也会变化
		uint32_t m_TradedVol;
		uint32_t m_LiveVol;
		BuySellType m_BuySellType; 
		OrderType m_OrderType;
		OrderStatusType m_OrderStatus;
		AT_Time m_UpdateTime;
		AT_Order_Key m_Key;
	};

}

