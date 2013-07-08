#pragma once
#include "IExecutor.h"
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

	class LimitExecutor :public IExecutor
	{
	public:
		LimitExecutor(const std::string& aConfigFile);
		LimitExecutor(LimitExecutorParma);
		virtual ~LimitExecutor(void);

		//输入1 来自于上层的交易信号
		virtual Command AddExecution(ExecutorInput aExecutorInput) override;
		virtual Command Abrot() override;

		//输入2 来自于执行层面
		virtual	Command OnMarketDepth(const AT::MarketData& aMarketDepth) override;
		virtual	Command OnRtnOrder(const  AT::OrderUpdate& apOrder)override;

		virtual	Command OnRtnTrade(const  AT::TradeUpdate& apTrade)override;
		virtual ExecutionStatus	GetExecutionStatus() override;
		virtual std::string GetExecutorID()  override;




	private:
		LimitExecutorParma ReadConfigFile(const std::string& aConfigFile);
		void InitFromParma(LimitExecutorParma);

		Command			BuildCommand(ExecutorInput aNewOrder);

		void SetupExecutionStatus( const AT::OrderUpdate &aOrder );

		AT_Order_Key				m_OrderKey;
		AT::OrderUpdate				m_TheOnlyOneLimitOrder;
		ExecutionStatus				m_ExecutionStatus;


		LimitExecutorParma			m_Parma;
		OrderPriceType				m_PriceType;
		bool switch_on;
		//AT_Time						m_StartTime;
		//int							m_TragetVol;
		//AT::MarketData				m_LastMarket;
		//bool						m_IsBuy;
		//int							m_OrderValidTime;
		//AT_Order_Key				m_DelOrderSet;
		//int							m_iBufferPoint;//缓冲点
		//std::string					m_InstrumentID;
	};

}