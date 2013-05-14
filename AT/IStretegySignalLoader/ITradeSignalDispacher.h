#pragma  once
#include "TradeSignal.h"
namespace AT
{
	class ITradeSignalDispacher
	{
		virtual void HandleTradeSignal(const TradeSignal& aCommand) = 0;

	};

}