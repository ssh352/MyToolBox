#pragma once


#include "AT_Struct.h"
namespace AT
{
	enum class ETradeState
	{
		START,
		READY,
		STOP,
	};

	class ITradeSpi
	{
	public:
		virtual void NotifyStateTD(ETradeState aErrorCode,const std::string& aErrorMsg) = 0;
		virtual void OnRtnOrder(const OrderUpdate& apOrder) = 0;
		virtual void OnRtnTrade(const TradeUpdate& apTrade)=0;
	};
}

