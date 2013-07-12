#pragma once

#include "IExecutor.h"
#include <string>
namespace AT
{

	struct StopLossSetting
	{
		int StopLossOffset;
	};

	class StopLossExecutor :public IExecutor
	{
	public:
		StopLossExecutor(const std::string& aConfigName);
		virtual ~StopLossExecutor(void);

		//输入1 来自于上层的交易信号
		virtual void AddExecution(ExecutorInput aExecutorInput) override;
		virtual void Abrot() override;

		//输入2 来自于执行层面
		virtual	void OnMarketDepth(const AT::MarketData& aMarketDepth) override;
		virtual	void OnRtnOrder(const  AT::OrderUpdate& apOrder)override;
		virtual	void OnRtnTrade(const  AT::TradeUpdate& apTrade)override;

		virtual ExecutionStatus	GetExecutionStatus() override{return m_ExecutionStatus;};
		virtual std::string GetExecutorID()  override {return m_ExecutorID;};

	private:
		Command BuildCommand( ExecutorInput aNewOrder );
		int		m_StopLossOffset;
		ExecutionStatus	m_ExecutionStatus;
		AT_Order_Key				m_OrderKey;
		AT::OrderUpdate				m_TheOnlyOneMarketOrder;

	};


}
