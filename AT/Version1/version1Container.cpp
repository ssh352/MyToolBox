#include "version1Container.h"
#include "IndexContainer.h"
#include "ITradeSignalProducer.h"
namespace AT
{


version1Container::version1Container(void)
{
}


version1Container::~version1Container(void)
{
}

void AT::version1Container::OnMarketDepth( const AT::MarketData& aMarketDepth )
{
	m_pIndexContaner->OnMarketDepth(aMarketDepth);

	std::vector<TradeSignal> lSignalResult;
	for(auto lpSignalProducer :m_TradeSignalProducerVec)
	{
		lSignalResult.push_back(lpSignalProducer->ProduceTradeSignal(aMarketDepth.m_UpdateTime));
	}



}


}