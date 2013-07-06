#pragma once
#include "AT_Struct.h"
#include "Signal.h"
namespace AT
{
	class ITradeSignalFliter
	{
	public:
		//包含两个步骤，第一步是合并，第二步是过滤
		virtual Signal FliterTradeSignal(std::vector<Signal> aList) = 0;

		//更新最新的时间与行情
		virtual void OnMarketDepth(const AT::MarketData&  aMarket) = 0;

		//这两个函数将会成对调用，总是相伴随，分成2个是为了更短小清晰
		virtual void UpdateProfit(int32_t aProfit,AT_Time aTime) = 0;//通知一个信号的盈利
		virtual void OnAccountCompleteSignal() = 0; //通知一个信号结束
	};
}