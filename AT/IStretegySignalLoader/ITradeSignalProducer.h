#pragma once
#include "MarketData.h"
#include "TradeSignal.h"
namespace AT
{


class ITradeSignalProducer
{
public:
	//构造函数得到历史行情
	//this module is work as Strategy loader before , select as 信号ID
	//第一版从外部传入指标实例
	virtual std::pair<bool,TradeSignal>  OnMarketDepth(const MarketData& aMarketDepth) = 0;
	 
};

}