#include "TradeSignalProducer.h"

namespace AT
{


TradeSignalProducer::~TradeSignalProducer(void)
{
}

TradeSignalProducer::TradeSignalProducer(const std::string& aConfigFile, IndexContainer* apIndexContainer )
	:m_pIndexContainer(apIndexContainer)
{

}

TradeSignal TradeSignalProducer::ProduceTradeSignal( const AT_Time& aNow )
{
	// todo 
		TradeSignal lret ;
		lret.m_TradeSignalSequence = 0;
		lret.m_TradeSignalType = 1;
		return lret;
}

}
