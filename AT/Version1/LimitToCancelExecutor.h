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

	//����1 �������ϲ�Ľ����ź�
	virtual Command AddExecution(ExecutorInput aExecutorInput) override;
	virtual Command Abrot() override;

	//����2 ������ִ�в���
	virtual	Command OnMarketDepth(const AT::MarketData& aMarketDepth) override;
	virtual	Command OnRtnOrder(const  AT::OrderUpdate& apOrder)override;

	virtual	Command OnRtnTrade(const  AT::TradeUpdate& apTrade)override;
	virtual ExecutionStatus	GetExecutionStatus() = 0;
	virtual std::string GetExecutorID()  override;




private:
	void InitFromConfigFile(const std::string& aConfigFile);

	AT_Order_Key				m_OrderKey;

	std::unique_ptr<LimitExecutor>	m_pLimitExecutor;	
	int								m_CancelTimeVol;
	AT_Time							m_EndTime;
};

}