#pragma once
#include "MarketData.h"
#include "TradeSignal.h"
namespace AT
{


class ITradeSignalProducer
{
public:
	virtual TradeSignal  ProduceTradeSignal(const AT_Time& aNow) = 0;
	 
};



}