#pragma once
#include "AT_Struct.h"
#include "TradeSignal.h"
namespace AT
{
	class ITradeSignalFliter
	{
	public:
		virtual bool FliterTradeSignal(const TradeSignal& signal) = 0;
		virtual void OnMarketDepth(const MarketData& aMakretData) = 0;
		virtual void UpdateProfit(int32_t aProfit,AT_Time aTime) = 0;

		//for TradeSignalExecutor
		virtual void NotifySignalExecutorState();
	};
}