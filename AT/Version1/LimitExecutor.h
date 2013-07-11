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

		//����1 �������ϲ�Ľ����ź�
		virtual void AddExecution(ExecutorInput aExecutorInput) override;
		virtual void Abrot() override;

		//����2 ������ִ�в���
		virtual	void OnMarketDepth(const AT::MarketData& aMarketDepth) override;
		virtual	void OnRtnOrder(const  AT::OrderUpdate& apOrder)override;

		virtual	void OnRtnTrade(const  AT::TradeUpdate& apTrade)override;
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
	};

}