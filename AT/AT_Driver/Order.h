#pragma once

#include "ConstVarDefine.h"

#pragma  pack(push , 4) //4�ֽڶ���
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
		//char m_MoreInfo[32];
	};

	struct CancelOrder
	{
		char AccoutID[cAccoutIDLength];
		AT_Order_Key m_Key;
	};


	struct  OrderUpdate
	{
		char InstrumentID[cInstrimentIDLength];
		char AccoutID[cAccoutIDLength];
		int32_t m_Price;
		uint32_t m_Vol;		//���ӵ������� �����ͨ��modify��С �������Ҳ��仯
		uint32_t m_TradedVol;
		uint32_t m_LiveVol;
		BuySellType m_BuySellType; 
		OrderType m_OrderType;
		OrderStatusType m_OrderStatus;
		AT_Order_Key m_Key;
	};

}
#pragma  pack(pop)

