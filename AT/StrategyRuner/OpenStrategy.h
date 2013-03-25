
#pragma once

#include <boost\date_time\posix_time\posix_time.hpp>
#include <map>
#include <string>
#include "IStrategy.h"

class OpenStrategy :public AT::IStrategy
{
public:
	OpenStrategy();
	virtual ~OpenStrategy();

	virtual void NotifyState(int aErrorCode){};
	virtual void SetStrategyPram(AT::StrategyPram* apStrParam){};
public:
	virtual void OnMarketDepth(const std::string& aMarketDepth);
	virtual void OnRtnOrder(const std::string& apOrder){};
	virtual void OnRtnTrade(const std::string& apTrade){};

private:

	std::map<boost::posix_time::time_duration,double> m_MarketCache;

};
