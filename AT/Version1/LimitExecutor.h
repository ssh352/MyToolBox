#pragma once
#include "ExecutorBase.h"
#include <set>
#include <string>
namespace AT
{

	struct LimitExecutorParma
	{
		std::string ExecutorID;
		char		PriceType;
		int			PriceOffset;
	};

	class LimitExecutor :public ExecutorBase
	{
	public:
		LimitExecutor(const std::string& aConfigFile);
		virtual ~LimitExecutor(void);

		//输入1 来自于上层的交易信号
		virtual void DoAddExecution(ExecutorInput aExecutorInput) override;
		virtual void DoAbrot() override;
		virtual	void DoOnMarketDepth(const AT::MarketData& aMarketDepth) override;
		virtual	void DoOnRtnOrder(const  AT::OrderUpdate& apOrder)override;
		virtual	void DoOnRtnTrade(const  AT::TradeUpdate& apTrade)override;


	private:
		LimitExecutorParma ReadConfigFile(const std::string& aConfigFile);
		void InitFromParma(LimitExecutorParma);

		Command			BuildCommand(ExecutorInput aNewOrder);
	//	ExecutionStatus				m_ExecutionStatusBase;


		LimitExecutorParma			m_Parma;
		OrderPriceType				m_PriceType;
	};

}