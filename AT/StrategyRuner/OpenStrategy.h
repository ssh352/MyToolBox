
#pragma once

#include <boost\date_time\posix_time\posix_time.hpp>
#include <map>
#include <string>
#include <functional>
#include "IStrategy.h"


class OpenStrategy :public AT::IStrategy
{
public:
	OpenStrategy();
	virtual ~OpenStrategy();

	virtual void NotifyState(const std::string& aErrorCode){};
	virtual void SetStrategyPram(const std::string& apStrParam){};
public:
	virtual void OnMarketDepth(const std::string& aMarketDepth);
	virtual void OnRtnOrder(const std::string& apOrder);
	virtual void OnRtnTrade(const std::string& apTrade);
	virtual void OnRtnPos(const std::string& aPos){};
	virtual void Start() {};
	virtual void Stop() {};

	void Reload();

public:
	void SendExitHandle(std::function<void(double)> aExitHandle)
	{
		m_ExitHandle = aExitHandle;
	}

private:

	std::map<boost::posix_time::time_duration,double> m_MarketCache;
	std::string m_ActiveOrder;
	std::function<void(double)> m_ExitHandle;
	bool m_isPlaceOrder ;
	double m_placePrice;

};

