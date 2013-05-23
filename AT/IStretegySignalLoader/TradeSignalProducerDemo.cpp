#include "TradeSignalProducerDemo.h"

#include "ISignalModule.h"
namespace  AT
{



TradeSignalProducerDemo::~TradeSignalProducerDemo(void)
{
}

TradeSignalProducerDemo::TradeSignalProducerDemo( AT::ISignalModule* aPSignal )
	: m_pSingalModule(aPSignal)
{

}

bool TradeSignalProducerDemo::OnMarketDepth( const MarketData& aMarketDepth )
{
	return m_pSingalModule->OnMarketDepth(aMarketDepth) == 1;
}


}