#pragma once

#include "ExecutorBase.h"
#include <string>
namespace AT
{
	class LocalTriggerExector :public ExecutorBase
	{
	public:
		LocalTriggerExector(const boost::property_tree::ptree& aConfigPtee);
		virtual ~LocalTriggerExector(void);

		//输入1 来自于上层的交易信号
		virtual void DoAddExecution(ExecutorInput aExecutorInput) override;
		virtual void DoAbrot() override;

		//输入2 来自于执行层面
		virtual	void DoOnMarketDepth(const AT::MarketData& aMarketDepth) override;
		virtual	void DoOnRtnOrder(const  AT::OrderUpdate& apOrder)override;
		virtual	void DoOnRtnTrade(const  AT::TradeUpdate& apTrade)override;

	private:

		enum class TriggerStatus
		{
			NotTrigger,
			Triggerd,
		};

		TriggerStatus m_Status;

		int		m_StopLossOffset;
		int		m_TriggerPrice;
		boost::shared_ptr<IExecutor>		m_TriggerExecutor;
		ExecutorInput						m_Input;

	};


}
