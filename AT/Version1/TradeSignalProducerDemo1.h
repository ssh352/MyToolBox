#pragma once

#include "ITradeSignalProducer.h"
#include <string>
namespace AT
{
class IndexContainer;

//todo config files

//load the logic from script as python
class TradeSignalProducerDemo1 :public ITradeSignalProducer
{
public:
	TradeSignalProducerDemo1(const std::string& aConfigFile,IndexContainer* apIndexContainer);
	virtual ~TradeSignalProducerDemo1(void);
	virtual TradeSignal  ProduceTradeSignal(const AT_Time& aNow) override;

private:
	IndexContainer* m_pIndexContainer;

	int m_Seqence;
};

}
