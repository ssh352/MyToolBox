#pragma once
#include "IExecutor.h"
#include <set>
#include <string>
namespace AT
{


class OpenMarketExecutor :public IExecutor
{
public:
	OpenMarketExecutor(const std::string& aConfig);

	virtual ~OpenMarketExecutor(void);

	//输入1 来自于上层的交易信号
	virtual Command AddExecution(ExecutorInput aExecutorInput) override;
	virtual Command Abrot() override;

	//输入2 来自于执行层面
	virtual	Command OnMarketDepth(const AT::MarketData& aMarketDepth) override;
	virtual	Command OnRtnOrder(const  AT::OrderUpdate& apOrder)override;
	virtual	Command OnRtnTrade(const  AT::TradeUpdate& apTrade)override;
	virtual ExecutionStatus	GetExecutionStatus() = 0;
	virtual std::string GetExecutorID()  override;

private:
	Command			BuildCommand(ExecutorInput aNewOrder);

	bool						m_IsBuy;
	std::set<AT_Order_Key>		m_SendOrderSet;
	ExecutionStatus				m_ExecutionStatus;
	
};

}