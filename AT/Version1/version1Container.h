#pragma once
#include "AT_Struct.h"
#include "TradeSignal.h"
#include "TradeAccountContainer.h"
#include <vector>
namespace AT
{
class IndexContainer;
class ITradeSignalProducer;
class ITradeSignalFliter;
class ITradeSignalExecutor;

class version1Container
{
public:
	version1Container(void);
	~version1Container(void);

	//feed data
	void OnMarketDepth(const AT::MarketData& aMarketDepth) ;
	 void OnRtnOrder(const  OrderUpdate& apOrder) ;
	 void OnRtnTrade(const  TradeUpdate& apTrade) ; 

	std::vector<TradeSignal> ProduceTradeSignal( const AT::MarketData& aTriggerMarket );

	void UpdateSubPartMarket( const AT::MarketData& aMarketDepth );

private:
	IndexContainer* m_pIndexContaner;
	std::vector<ITradeSignalProducer* > m_TradeSignalProducerVec;
	ITradeSignalFliter*					m_pTradeSignalFliter;

	TradeAccountContainer				m_TradeAccountContaner;

};

}