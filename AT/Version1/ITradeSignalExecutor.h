#pragma  once
#include "AT_Struct.h"
#include "TradeSignal.h"
namespace AT
{
	class ITradeSignalExecutor 
	{
	
	public:

		//输入1 来自于上层的交易信号
		virtual void HandleTradeSignal(const TradeSignal& aSignal) =0;

		//输入2 行情信号
		virtual void OnMarketDepth(const AT::MarketData& aMarketDepth) =0;

		//输入3 来自于下游模块的通知
		virtual void NotifyTraded(const TradeSignal& aSignal , int32_t accountIndex) =0;
		virtual void NotifyCancel(const TradeSignal& aSignal , int32_t accountIndex) = 0;
		//void NotifyTrade(const TradeSignal& aSignal , int32_t accountIndex);

	};


}