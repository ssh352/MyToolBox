#include "TradeSignalProducerDemo.h"

#include "ISignalModule.h"
namespace  AT
{



TradeSignalProducerDemo::~TradeSignalProducerDemo(void)
{
}

TradeSignalProducerDemo::TradeSignalProducerDemo( AT::ISignalModule* aPSignal )
	: m_pSingalModule(aPSignal)
	, m_Sequence(0)
{

}

std::pair<bool,TradeSignal> TradeSignalProducerDemo::OnMarketDepth( const MarketData& aMarketDepth )
{
	std::pair<bool,TradeSignal> lret;
	lret.first =  (1 == m_pSingalModule->OnMarketDepth(aMarketDepth) );
	if(lret.first)
	{
		lret.second.m_TradeSignalSequence = ++m_Sequence;
		lret.second.m_TradeSignalType =  1;
	}

	return lret;

}


}