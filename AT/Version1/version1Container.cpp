#include "version1Container.h"
#include "IndexContainer.h"
#include "ITradeSignalProducer.h"
#include "ITradeSignalFliter.h"
#include "ITradeSignalExecutor.h"
#include <boost\bind.hpp>
namespace AT
{


version1Container::version1Container(const char* aConfigFile, AT::IDriver_TD* apTD, AT::IStrategySpi* aStrSpi, const  AT::IMarketCache* apMarketCache)
	:m_pTD(apTD)
	,m_MarketCache(apMarketCache)
	,m_TradeAccountContaner(aConfigFile,apTD)
{
////todo init
//	IndexContainer* m_pIndexContaner;
//	std::vector<ITradeSignalProducer* > m_TradeSignalProducerVec;
//	ITradeSignalFliter*					m_pTradeSignalFliter;
	 InitIndexContainer();
	 InitFliter();
	 InitAccountContainer();

}


version1Container::~version1Container(void)
{
}

void version1Container::OnMarketDepth( const AT::MarketData& aMarketDepth )
{
	UpdateSubPartMarket(aMarketDepth);

	std::vector<TradeSignal> lTradeSignalVec = ProduceTradeSignal(aMarketDepth);

	TradeSignal lFinalSignal = m_pTradeSignalFliter->FliterTradeSignal(lTradeSignalVec);

	m_TradeAccountContaner.OnMarketDepth(aMarketDepth);


}

std::vector<TradeSignal> version1Container::ProduceTradeSignal( const AT::MarketData& lNow )
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
	m_TradeAccountContaner.OnMarketDepth(aMarketDepth);
}

void version1Container::OnRtnOrder( const OrderUpdate& apOrder )
{
	m_TradeAccountContaner.OnRtnOrder(apOrder);
}

void version1Container::OnRtnTrade( const TradeUpdate& apTrade )
{
	m_TradeAccountContaner.OnRtnTrade(apTrade);
}

void version1Container::InitIndexContainer()
{

}

void version1Container::InitAccountContainer()
{
	m_TradeAccountContaner.SetProfitCallback(boost::bind(&ITradeSignalFliter::UpdateProfit,m_pTradeSignalFliter,_1,_2));
}

void version1Container::InitFliter()
{

}

}