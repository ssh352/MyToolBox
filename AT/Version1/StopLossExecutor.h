#pragma once

#include "ExecutorBase.h"
#include <string>
namespace AT
{

	struct StopLossSetting
	{
		int StopLossOffset;
	};

	class StopLossExecutor :public ExecutorBase
	{
	public:
		StopLossExecutor(const std::string& aConfigName);
		virtual ~StopLossExecutor(void);

		//输入1 来自于上层的交易信号
		virtual void DoAddExecution(ExecutorInput aExecutorInput) override;
		virtual void DoAbrot() override;

		//输入2 来自于执行层面
		virtual	void DoOnMarketDepth(const AT::MarketData& aMarketDepth) override;
		virtual	void DoOnRtnOrder(const  AT::OrderUpdate& apOrder)override;
		virtual	void DoOnRtnTrade(const  AT::TradeUpdate& apTrade)override;

	private:
		Command BuildCommand( ExecutorInput aNewOrder );
		int		m_StopLossOffset;

	};


}
