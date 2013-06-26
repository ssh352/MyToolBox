#pragma once
#include "ITradeAccount.h"
#include "IDriver_TD.h"
#include <boost\smart_ptr.hpp>
namespace AT
{
typedef  boost::function<void(int32_t profix,AT_Time aTime)> FliterProfitUpdater; 

class TradeAccountContainer
{
public:
	TradeAccountContainer(const char* configFile, AT::IDriver_TD* apTD );
	~TradeAccountContainer(void);

	void HandleTradeSignalProfit(int32_t aProfit,AT_Time aTime ,ITradeAccount* sender );
	void HandleTradeSignal(const TradeSignal& aTradeSignal);
	 void OnMarketDepth(const MarketData& aMarketDepth);
	 void OnRtnOrder(const  OrderUpdate& apOrder) ;
	 void OnRtnTrade(const  TradeUpdate& apTrade) ;  
	 void SetProfitCallback(FliterProfitUpdater aFliterProfitUpdater)
	{
		m_ProfitNotifyer = aFliterProfitUpdater;
	};

	 void HandleOneAccountProfit(int32_t aProfit,AT_Time aTime ,ITradeAccount* sender ) ;

private:

	std::vector<boost::shared_ptr<ITradeAccount> > m_AccountList;
	FliterProfitUpdater m_ProfitNotifyer;
	struct  AccountProfitStatus
	{
		bool isFinished;
		int32_t m_Profit;
		AT_Time m_Time ;
	};
	std::map<boost::shared_ptr<ITradeAccount>,AccountProfitStatus > m_AccountFinishedList;

};

}