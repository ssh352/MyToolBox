#pragma once
#include "IExecutor.h"
#include <set>
#include <string>
namespace AT
{

class LimitExecutor;
class LimitToCancelExecutor :public IExecutor
{
public:
	LimitToCancelExecutor(const std::string& aConfigFile);
	virtual ~LimitToCancelExecutor(void);

	//输入1 来自于上层的交易信号
	virtual void AddExecution(ExecutorInput aExecutorInput) override;
	virtual void Abrot() override;

	//输入2 来自于执行层面
	virtual	void OnMarketDepth(const AT::MarketData& aMarketDepth) override;
	virtual	void OnRtnOrder(const  AT::OrderUpdate& apOrder)override;

	virtual	void OnRtnTrade(const  AT::TradeUpdate& apTrade)override;
	virtual ExecutionStatus	GetExecutionStatus() override;
	virtual std::string GetExecutorID()  override;




private:
	void InitFromConfigFile(const std::string& aConfigFile);

	AT_Order_Key				m_OrderKey;

	std::unique_ptr<LimitExecutor>	m_pLimitExecutor;	
	int								m_CancelTimeVol;
	AT_Time							m_EndTime;
};

}