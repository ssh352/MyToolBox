#pragma once

#include "ConstVarDefine.h"
#pragma  pack(push , 8)
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
		char m_SenderInfo[32];//info which startgy is
		AT_Order_Key m_Key;

		InputOrder()
		{
			memset(InstrumentID,0,sizeof(InstrumentID));
			memset(AccoutID,0,sizeof(AccoutID));
			memset(m_MoreInfo,0,sizeof(m_MoreInfo));
			memset(m_SenderInfo,0,sizeof(m_SenderInfo));
		}
		std::string ToString()const;
	};

	struct CancelOrder
	{
		AT_Order_Key m_Key;
		std::string ToString() const;
		CancelOrder()
		{

		}
	};

	struct ModifyOrder
	{
		int32_t m_Price;
		uint32_t m_Vol;
		AT_Order_Key m_Key;
		//todo  whether should we set the orgi price and vol for avoid cross modify
		//int32_t m_OrgiPrice;
		//uint32_t m_OrgiVol;

		ModifyOrder()
		{

		}
		std::string ToString() const;
	};


	struct  OrderUpdate
	{
		char InstrumentID[cInstrimentIDLength];
		char AccoutID[cAccoutIDLength];
		char m_PlatformOrderID[cPlatformOrderIDLength];
		char m_ExchangeOrderID[cExchangeOrderIDLength];
		char m_ErrorMessage[cErrorMessageLength];
		int32_t m_Price;
		uint32_t m_Vol;		//单子的总数量 ，如果通过modify改小 这个数量也会变化
		uint32_t m_TradedVol;
		uint32_t m_LiveVol;
		BuySellType m_BuySellType; 
		OrderType m_OrderType;
		OpenCloseType m_OpenCloseType;
		OrderStatusType m_OrderStatus;
		AT_Time m_UpdateTime;
		AT_Order_Key m_Key;
		OrderUpdate()
		{
			memset(InstrumentID,0,sizeof(InstrumentID));
			memset(AccoutID,0,sizeof(AccoutID));
			memset(m_PlatformOrderID,0,sizeof(m_PlatformOrderID));
			memset(m_ErrorMessage,0,sizeof(m_ErrorMessage));
			memset(m_ExchangeOrderID,0,sizeof(m_ExchangeOrderID));
		}
		std::string ToString() const;
	};

	

	void CopyOrderFields(AT::OrderUpdate& aOrderUpdate,const AT::InputOrder& aSrcOrder);

}

#pragma  pack(pop)