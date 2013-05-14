#pragma  once
#include "AT_Struct.h"
#include "TradeSignal.h"
namespace AT
{
	class ITradeSignalExecutor 
	{
		//输入1 来自于上层的交易信号
		void HandleTradeSignal(const TradeSignal& aSignal);

		//输入2 行情信号
		void OnMarketDepth(const AT::MarketData& aMarketDepth);

		//输入3 来自于下游模块的通知
		void NotifyTraded(const TradeSignal& aSignal , int32_t accountIndex);
		void NotifyCancel(const TradeSignal& aSignal , int32_t accountIndex);
		//void NotifyTrade(const TradeSignal& aSignal , int32_t accountIndex);

	};


}