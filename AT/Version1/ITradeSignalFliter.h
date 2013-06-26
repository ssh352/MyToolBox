#pragma once
#include "AT_Struct.h"
#include "TradeSignal.h"
namespace AT
{
	class ITradeSignalFliter
	{
	public:
		virtual TradeSignal FliterTradeSignal(std::vector<TradeSignal> aList) = 0;

		virtual void OnMarketDepth(const AT::MarketData&  aMarket) = 0;
		virtual void UpdateProfit(int32_t aProfit,AT_Time aTime) = 0;

	//	//for TradeSignalExecutor
	//	virtual void NotifySignalExecutorState();
	};
}