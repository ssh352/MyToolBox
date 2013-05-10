#pragma  once
#include "TradeSignal.h"
namespace AT
{
	class ITradeSignalExecutor
	{
		void HandleTradeSignal(const TradeSignal& aSignal);
	};
}