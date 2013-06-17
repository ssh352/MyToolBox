#include "TradeSignalProducerDemo1.h"

namespace AT
{


TradeSignalProducerDemo1::~TradeSignalProducerDemo1(void)
{
}

TradeSignalProducerDemo1::TradeSignalProducerDemo1(const std::string& aConfigFile, IndexContainer* apIndexContainer )
	:m_pIndexContainer(apIndexContainer)
{

}

TradeSignal TradeSignalProducerDemo1::ProduceTradeSignal( const AT_Time& aNow )
{
	// todo 
		TradeSignal lret ;
		lret.m_TradeSignalSequence = 0;
		lret.m_TradeSignalType = 1;
		return lret;
}

}
