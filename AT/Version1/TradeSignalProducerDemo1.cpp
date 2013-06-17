#include "TradeSignalProducerDemo1.h"
#include "IndexContainer.h"
namespace AT
{


TradeSignalProducerDemo1::~TradeSignalProducerDemo1(void)
{
}

TradeSignalProducerDemo1::TradeSignalProducerDemo1(const std::string& aConfigFile, IndexContainer* apIndexContainer )
	:m_pIndexContainer(apIndexContainer)
	,m_Seqence(0)
{

}

AT::TradeSignal TradeSignalProducerDemo1::ProduceTradeSignal( const MarketData& aTriggerMarket )
{
		TradeSignal lret ;
		if(m_pIndexContainer->GetIndexCount("HKY006",1,aTriggerMarket.m_UpdateTime,aTriggerMarket.m_UpdateTime) >= 1)
		{
			lret.m_Valid = true;
			lret.m_TradeSignalSequence = 0;
			lret.m_TradeSignalType = m_Seqence++;
			return lret;
		}
		else
		{
			lret.m_Valid = false;
			return lret;
		}
}

}
