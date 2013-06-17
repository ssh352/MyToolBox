#include "version1Container.h"
#include "IndexContainer.h"
#include "ITradeSignalProducer.h"
#include "ITradeSignalFliter.h"
#include "ITradeSignalExecutor.h"
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
	UpdateSubPartMarket(aMarketDepth);

	AT_Time lNow = aMarketDepth.m_UpdateTime;

	std::vector<TradeSignal> lTradeSignalVec = ProduceTradeSignal(lNow);


	TradeSignal lFinalSignal = m_pTradeSignalFliter->FliterTradeSignal(lTradeSignalVec);

	//m_pTradeSignalExecutor->HandleTradeSignal(lFinalSignal);

}

std::vector<TradeSignal> AT::version1Container::ProduceTradeSignal( AT_Time lNow )
{
	std::vector<TradeSignal> lSignalResult;
	for(auto lpSignalProducer :m_TradeSignalProducerVec)
	{
		lSignalResult.push_back(lpSignalProducer->ProduceTradeSignal(lNow));
	}
	return lSignalResult;
}

void version1Container::UpdateSubPartMarket( const AT::MarketData& aMarketDepth )
{
	m_pIndexContaner->OnMarketDepth(aMarketDepth);
	m_pTradeSignalFliter->OnMarketDepth(aMarketDepth);
	m_pTradeSignalExecutor->OnMarketDepth(aMarketDepth);
}

}