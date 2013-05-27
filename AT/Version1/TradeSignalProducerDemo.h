#pragma once
#include "ITradeSignalProducer.h"

namespace AT
{
class ISignalModule;
class TradeSignalProducerDemo : public ITradeSignalProducer
{
public:
	TradeSignalProducerDemo(AT::ISignalModule* aPSignal);
	virtual ~TradeSignalProducerDemo(void);

	virtual TradeSignal OnMarketDepth(const MarketData& aMarketDepth) override ;
private: 
	AT::ISignalModule* m_pSingalModule;
	uint16_t			m_Sequence;
};

}