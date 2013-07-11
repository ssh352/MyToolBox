#pragma once
#include "IExecutor.h"
#include <set>
#include <string>
#include <map>
namespace AT
{


class MarketExecutor :public IExecutor
{
public:
	MarketExecutor(const std::string& aConfig);

	virtual ~MarketExecutor(void);

	//输入1 来自于上层的交易信号
	virtual void AddExecution(ExecutorInput aExecutorInput) override;
	virtual void Abrot() override;

	//输入2 来自于执行层面
	virtual	void OnMarketDepth(const AT::MarketData& aMarketDepth) override;
	virtual	void OnRtnOrder(const  AT::OrderUpdate& apOrder)override;

	void SetupExecutionStatus( const AT::OrderUpdate &aOrder );

	virtual	void OnRtnTrade(const  AT::TradeUpdate& apTrade)override;
	virtual ExecutionStatus	GetExecutionStatus() override;
	virtual std::string GetExecutorID()  override;

private:
	Command			BuildCommand(ExecutorInput aNewOrder);
	AT_Order_Key				m_OrderKey;
	AT::OrderUpdate				m_TheOnlyOneMarketOrder;
	ExecutionStatus				m_ExecutionStatus;
	
};

}