#include "version1Container.h"
#include "IndexContainer.h"
#include "ITradeSignalProducer.h"
#include "ITradeSignalFliter.h"
namespace AT
{


version1Container::version1Container(void)
{
////todo init
//	IndexContainer* m_pIndexContaner;
//	std::vector<ITradeSignalProducer* > m_TradeSignalProducerVec;
//	ITradeSignalFliter*					m_pTradeSignalFliter;
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

	TradeSignal lFinalSignal = m_pTradeSignalFliter->FliterTradeSignal(lSignalResult);


}


}