
#pragma once

#include <boost\date_time\posix_time\posix_time.hpp>
#include <map>
#include <string>
#include <functional>
#include "IStrategy.h"


class CloseStrategy :public AT::IStrategy
{
public:
	CloseStrategy();
	virtual ~CloseStrategy();

	virtual void NotifyState(int aErrorCode);
	virtual void SetStrategyPram(AT::StrategyPram* apStrParam){};
public:
	virtual void OnMarketDepth(const std::string& aMarketDepth);
	virtual void OnRtnOrder(const std::string& apOrder);
	virtual void OnRtnTrade(const std::string& apTrade);
	virtual void OnRtnPos(const std::string& aPos){};

	void Reload();

public:
	void SendExitHandle(std::function<void()> aExitHandle)
	{
		m_ExitHandle = aExitHandle;
	}
	void SetStartPrice(double aStartPrice)
	{
		m_StartPrice = aStartPrice;
	}

private:

	std::map<boost::posix_time::time_duration,double> m_MarketCache;
	std::string m_ActiveOrder;
	std::function<void()> m_ExitHandle;
	bool m_isPlaceOrder ;
	double m_StartPrice;

};

