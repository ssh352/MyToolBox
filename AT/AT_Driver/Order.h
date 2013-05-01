#pragma once

#include "ConstVarDefine.h"

namespace AT
{
	struct InputOrder
	{
		//without null end
		char InstrumentID[cInstrimentIDLength];
		char AccoutID[cAccoutIDLength];
		int32_t m_Price;
		uint32_t m_Vol;
		BuySellType m_BuySellType; 
		OrderType m_OrderType;
		OpenCloseType m_OpenCloseType;
		char m_MoreInfo[32];
		char m_SendeeInfo[32];//info which startgy is
		AT_Order_Key m_Key;
	};

	struct CancelOrder
	{
		char AccoutID[cAccoutIDLength];
		AT_Order_Key m_Key;
	};

	struct ModifyOrder
	{
		int32_t m_Price;
		uint32_t m_Vol;
		//todo  whether should we set the orgi price and vol for avoid cross modify
		//int32_t m_OrgiPrice;
		//uint32_t m_OrgiVol;
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

