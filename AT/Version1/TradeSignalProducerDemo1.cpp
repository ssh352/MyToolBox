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

TradeSignal TradeSignalProducerDemo1::ProduceTradeSignal( const AT_Time& aNow )
{
		if(m_pIndexContainer->GetIndexCount("HKY006",1,aNow,aNow) >= 1)
		{
			TradeSignal lret ;
			lret.m_TradeSignalSequence = 0;
			lret.m_TradeSignalType = m_Seqence++;
			return lret;
		}
}

}
