
#pragma once

#include <boost\date_time\posix_time\posix_time.hpp>
#include <map>
#include <string>
#include <functional>
#include "IStrategy.h"


class OpenStrategy :public AT::IStrategy
{
public:
	OpenStrategy(AT::IDriver_TD* apTD);
	virtual ~OpenStrategy();


	virtual void UpdateParam( AT::IStrategy::EStrInputState aSource,const std::string& apStrParam);
public:
	virtual void OnMarketDepth(const std::string& aMarketDepth);
	virtual void OnRtnOrder(const std::string& apOrder);
	virtual void OnRtnTrade(const std::string& apTrade);
	virtual void OnRtnPos(const std::string& aPos){};
	virtual void Start() {};
	virtual void Stop() {};

	void Reload();

public:
	void SendExitHandle(std::function<void(double,bool buy)> aExitHandle)
	{
		m_ExitHandle = aExitHandle;
	}

private:

	std::map<boost::posix_time::time_duration,double> m_MarketCache;
	std::string m_ActiveOrder;
	std::function<void(double,bool)> m_ExitHandle;
	bool m_isPlaceOrder ;
	double m_placePrice;
	bool m_IsSell;

	 std::string m_Instument ;
	 int   m_TriggerTimeSeconds ;
	 double m_TriigerPrice ;
	 AT::IDriver_TD* m_pTD;

};

