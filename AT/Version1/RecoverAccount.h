#pragma once
#include "IAccount.h"
#include <string>
namespace AT
{


class RecoverAccount :public IAccount
{
public:
	RecoverAccount(const std::string& aConfigFile);
	virtual ~RecoverAccount(void);
	virtual void HandleTradeSignal(const Signal& aTradeSignal) override;
	virtual void OnMarketDepth(const MarketData& aMarketDepth) override;
	virtual void OnRtnOrder(const  OrderUpdate& apOrder) override;
	virtual void OnRtnTrade(const  TradeUpdate& apTrade) override;  
private:
	const std::string	m_ConfigFile;
	
};

}