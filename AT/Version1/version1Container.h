#pragma once
#include "AT_Struct.h"
#include "IStrategy.h"
#include "Signal.h"
#include "AccountContainer.h"
#include <vector>
#include <windows.h>
namespace AT
{
class IndexContainer;
class ISignalProducer;
class ITradeSignalFliter;
class ITradeSignalExecutor;

class version1Container :public IStrategy
{
public:
	version1Container(const char* aConfigFile, AT::IDriver_TD* apTradeSpi, AT::IStrategySpi* aStrSpi, const  AT::IMarketCache* apMarketCache);
	~version1Container(void);

	//feed data
	virtual void OnMarketDepth(const AT::MarketData& aMarketDepth) override;
	virtual  void OnRtnOrder(const  OrderUpdate& apOrder)override ;
	 virtual void OnRtnTrade(const  TradeUpdate& apTrade) override; 

	 virtual void UpdateParam(EStrInputState errCode ,const Param& apParam) override{};
	 virtual void Start()override;
	 virtual void Stop() override;

private:

	std::vector<Signal> ProduceSignal( const AT::MarketData& aTriggerMarket );

	void UpdateSubPartMarket( const AT::MarketData& aMarketDepth );

	void InitIndexContainer();
	void InitFliter();
	void InitAccountContainer();
	void InitSignalProducer();

private:
	IndexContainer* m_pIndexContaner;
	std::vector<ISignalProducer* > m_TradeSignalProducerVec;
	ITradeSignalFliter*					m_pTradeSignalFliter;
	AccountContainer				m_TradeAccountContaner;
private:
	AT::IDriver_TD* m_pTD;
	const AT::IMarketCache*				m_pMarketCache;

	std::vector<HMODULE>				m_LibHandleVec;
};

}