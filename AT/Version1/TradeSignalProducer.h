#pragma once

#include "ITradeSignalProducer.h"
#include <string>
namespace AT
{
class IndexContainer;

//todo config files
class TradeSignalProducer :public ITradeSignalProducer
{
public:
	TradeSignalProducer(const std::string& aConfigFile,IndexContainer* apIndexContainer);
	virtual ~TradeSignalProducer(void);
	virtual TradeSignal  ProduceTradeSignal(const AT_Time& aNow) override;

private:
	IndexContainer* m_pIndexContainer;

	int m_Seqence;
};

}
