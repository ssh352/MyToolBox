#pragma once


#include "AT_Struct.h"
namespace AT
{
	enum class ETradeState
	{
		START,
		READY,
		DISCONNCT,
		STOP,
	};

	class ITradeSpi
	{
	public:
		virtual void NotifyStateTD(ETradeState aErrorCode,const char* aErrorMsg) = 0;
		virtual void OnRtnOrder(const OrderUpdate& apOrder) = 0;
		virtual void OnRtnTrade(const TradeUpdate& apTrade)=0;
	};
}

