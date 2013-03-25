#pragma once
#include "IStrategy.h"
#include "OpenStrategy.h"
class StrDemo1 :public AT::IStrategy
{
public:
	StrDemo1();
	virtual ~StrDemo1();

	virtual void NotifyState(int aErrorCode);
	virtual void SetStrategyPram(AT::StrategyPram* apStrParam);
public:
	virtual void OnMarketDepth(const std::string& aMarketDepth);
	virtual void OnRtnOrder(const std::string& apOrder);
	virtual void OnRtnTrade(const std::string& apTrade);
	virtual void OnRtnPos(const std::string& aPos);

	void SetChildTD()
	{
		m_openSate.SetTDPoint(m_pTD);
	}
	

private:
	IStrategy* m_Handler; //zhuang  tai mo shi
	OpenStrategy m_openSate;
};

