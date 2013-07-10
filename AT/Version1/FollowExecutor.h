#pragma once
#include "IExecutor.h"
#include <set>
#include <string>
#include <map>
namespace AT
{


class FollowExecutor :public IExecutor
{
public:
	FollowExecutor(const std::string& aConfig);

	virtual ~FollowExecutor(void);

	//输入1 来自于上层的交易信号
	virtual Command AddExecution(ExecutorInput aExecutorInput) override;
	virtual Command Abrot() override;

	//输入2 来自于执行层面
	virtual	Command OnMarketDepth(const AT::MarketData& aMarketDepth) override;
	virtual	Command OnRtnOrder(const  AT::OrderUpdate& apOrder)override;

	void SetupExecutionStatus( const AT::OrderUpdate &aOrder );

	virtual	Command OnRtnTrade(const  AT::TradeUpdate& apTrade)override;
	virtual ExecutionStatus	GetExecutionStatus()override;
	virtual std::string GetExecutorID()  override;

private:
	Command			BuildCommand(int vol);
	AT_Order_Key				m_OrderKey;
	AT::OrderUpdate				m_TheOnlyOneMarketOrder;
	ExecutionStatus				m_ExecutionStatus;
	bool						m_IsAbrot;

	//
	BuySellType		m_BuySell;
	OpenCloseType	m_OpenClose;
	std::string		m_InstrumentID;
	
};

}