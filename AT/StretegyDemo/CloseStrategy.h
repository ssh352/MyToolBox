
#pragma once

#include <boost\date_time\posix_time\posix_time.hpp>
#include <map>
#include <string>
#include <functional>
#include "IStrategy.h"


class CloseStrategy :public AT::IStrategy
{
public:
	CloseStrategy(AT::IDriver_TD* apTD);
	virtual ~CloseStrategy();

public:
	virtual void NotifyState(const std::string& aErrorCode){};
	virtual void SetStrategyPram(const std::string& apStrParam);
public:
	virtual void OnMarketDepth(const std::string& aMarketDepth);
	virtual void OnRtnOrder(const std::string& apOrder);
	virtual void OnRtnTrade(const std::string& apTrade);
	virtual void OnRtnPos(const std::string& aPos) {};

	virtual void Start() {};
	virtual void Stop() {};

	void Reload();

public:
	void SendExitHandle(std::function<void()> aExitHandle)
	{
		m_ExitHandle = aExitHandle;
	}
	void SetStartPrice(double aStartPrice,bool isSell);

private:

	std::map<boost::posix_time::time_duration,double> m_MarketCache;
	std::string m_ActiveOrder;
	std::function<void()> m_ExitHandle;
	bool m_isPlaceOrder ;
	double m_StartPrice;
	int m_CheckLevel;

private:
	bool CheckLevel0(double aPrice);
	bool CheckLevel1(double aPrice);
	bool CheckLevel2(double aPrice);
	bool CheckLevel3(double aPrice);



	bool CheckTrigger(double aTrigger);

	bool m_IsSell;


	 std::string m_Instument ;

	  double m_Level0EnterPrice;
	 double m_Level1EnterPrice;
	 double m_Level2EnterPrice;
	 double m_Level3EnterPrice;

	   double m_Level0QuitPrice;
	 double m_Level1QuitPrice;
	 double m_Level2QuitPrice;
	 double m_Level3QuitPrice;
	


};

