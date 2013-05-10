#pragma  once
#include "TradeSignal.h"
namespace AT
{
	class ITradeAccount
	{
		void HandleTradeSignal(const AT::TradeSignal& aSignal);
	};
}