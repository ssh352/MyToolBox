#pragma once
#include "MarketData.h"
#include "TradeSignal.h"
namespace AT
{


class ISignalProducer
{
public:
	virtual Signal  ProduceSignal(const MarketData& aTriggerMakret) = 0;
	 
};



}