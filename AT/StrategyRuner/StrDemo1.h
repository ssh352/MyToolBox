#pragma once
#include "IStrategy.h"
#include "OpenStrategy.h"
#include "CloseStrategy.h"
#include <string>
class StrDemo1 :public AT::IStrategy
{
public:
	StrDemo1();
	virtual ~StrDemo1();

	virtual void NotifyState(const std::string& aErrorCode);
	virtual void SetStrategyPram(const std::string& apStrParam);
public:
	virtual void OnMarketDepth(const std::string& aMarketDepth);
	virtual void OnRtnOrder(const std::string& apOrder);
	virtual void OnRtnTrade(const std::string& apTrade);
	virtual void OnRtnPos(const std::string& aPos);

	void SetupChild()
	{
		m_openState.SetTDPoint(m_pTD);
		m_openState.SendExitHandle(std::bind(&StrDemo1::ChangeToClostState,this,std::placeholders::_1));
		m_clostState.Reload();

		m_clostState.SetTDPoint(m_pTD);
		m_clostState.SendExitHandle(std::bind(&StrDemo1::ChangeToOpenState,this));
		m_clostState.Reload();
	}
	
	void ChangeToOpenState()
	{
		m_CurrentState = &m_openState;
		m_openState.Reload();
	}

	void ChangeToClostState(double aPrice)
	{
		m_CurrentState = &m_clostState;
		m_clostState.Reload();
		m_clostState.SetStartPrice(aPrice);
	}

private:
	IStrategy* m_CurrentState;
	OpenStrategy m_openState;
	CloseStrategy m_clostState;
};

