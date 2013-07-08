#pragma once
#include "IExecutor.h"
#include <set>
#include <string>
#include <map>
namespace AT
{


class OpenMarketExecutor :public IExecutor
{
public:
	OpenMarketExecutor(const std::string& aConfig);

	virtual ~OpenMarketExecutor(void);

	//����1 �������ϲ�Ľ����ź�
	virtual Command AddExecution(ExecutorInput aExecutorInput) override;
	virtual Command Abrot() override;

	//����2 ������ִ�в���
	virtual	Command OnMarketDepth(const AT::MarketData& aMarketDepth) override;
	virtual	Command OnRtnOrder(const  AT::OrderUpdate& apOrder)override;

	void SetupExecutionStatus( const AT::OrderUpdate &aOrder );

	virtual	Command OnRtnTrade(const  AT::TradeUpdate& apTrade)override;
	virtual ExecutionStatus	GetExecutionStatus() = 0;
	virtual std::string GetExecutorID()  override;

private:
	Command			BuildCommand(ExecutorInput aNewOrder);

	bool						m_IsBuy;
	AT_Order_Key				m_OrderKey;
	AT::OrderUpdate				m_TheOnlyOneMarketOrder;
	ExecutionStatus				m_ExecutionStatus;
	
};

}