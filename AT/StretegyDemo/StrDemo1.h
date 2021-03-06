#pragma once
#include "IStrategy.h"
#include "OpenStrategy.h"
#include "CloseStrategy.h"
#include <string>
namespace AT
{
	class IStrategySpi;
}

class StrDemo1 :public AT::IStrategy
{
public:

	StrDemo1(const std::string& aConfigFile,AT::IDriver_TD * apTD  , AT::IStrategySpi* apStrSpi);
	virtual ~StrDemo1();

	virtual void UpdateParam(EStrInputState errCode ,const std::string& apParam) override;
	virtual void Start() override  {m_isRuning = true;};
	virtual void Stop()  override {m_isRuning = false;};
public:
	virtual void OnMarketDepth(const std::string& aMarketDepth)override;
	virtual void OnRtnOrder(const std::string& apOrder)override;
	virtual void OnRtnTrade(const std::string& apTrade)override;
	virtual void OnRtnPos(const std::string& aPos)override;

private:
	void SetupChild();

	void ChangeToOpenState()
	{
		m_openState.Reload();
		m_CurrentState = &m_openState;
	}

	void ChangeToClostState(double aPrice,bool isSell)
	{
		m_clostState.Reload();
		m_clostState.SetStartPrice(aPrice,isSell);
		m_CurrentState = &m_clostState;
	}

private:
	bool		m_isRuning;
	IStrategy* m_CurrentState;
	OpenStrategy m_openState;
	CloseStrategy m_clostState;
	std::string m_ConfigFileName;
};

