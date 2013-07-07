#pragma once
#include "IAccount.h"
#include "IDriver_TD.h"
#include <boost\smart_ptr.hpp>
namespace AT
{
typedef  boost::function<void(int32_t profix,AT_Time aTime)> FliterProfitUpdater; 

class AccountContainer
{
public:
	AccountContainer(const char* configFile, AT::IDriver_TD* apTD );
	~AccountContainer(void);

	void HandleTradeSignalProfit(int32_t aProfit,AT_Time aTime ,IAccount* sender );
	void HandleTradeSignal(const Signal& aTradeSignal);
	 void OnMarketDepth(const MarketData& aMarketDepth);
	 void OnRtnOrder(const  OrderUpdate& apOrder) ;
	 void OnRtnTrade(const  TradeUpdate& apTrade) ;  
	// void SetProfitCallback(FliterProfitUpdater aFliterProfitUpdater)
	//{
	//	m_ProfitNotifyer = aFliterProfitUpdater;
	//};

	 void HandleOneAccountProfit(int32_t aProfit,AT_Time aTime ,IAccount* sender ) ;

private:

	boost::shared_ptr<AT::IAccount> CreateAccount(const std::string& aAccountType, const std::string& aConfigFile);
	std::vector<boost::shared_ptr<AT::IAccount> > m_AccountList;
	 AT::IDriver_TD* m_pTD;
	//FliterProfitUpdater m_ProfitNotifyer;
	//struct  AccountProfitStatus
	//{
	//	bool isFinished;
	//	int32_t m_Profit;
	//	AT_Time m_Time ;
	//};
	//std::map<boost::shared_ptr<IAccount>,AccountProfitStatus > m_AccountFinishedList;

};

}