#pragma once
#include "MarketData.h"
#include "TradeSignal.h"
namespace AT
{


class ITradeSignalProducer
{
public:
	virtual TradeSignal  ProduceTradeSignal(const MarketData& aTriggerMakret) = 0;
	 
};



}