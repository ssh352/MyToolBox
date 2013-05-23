#pragma once
#include "MarketData.h"
namespace AT
{


class ITradeSignalProducer
{
public:
	//this module is work as Strategy loader before , select as 信号ID
	//第一版从外部传入指标实例
	virtual bool OnMarketDepth(const MarketData& aMarketDepth) = 0;

};

}