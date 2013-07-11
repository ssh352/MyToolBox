#pragma once
#include "IExecutor.h"
#include <memory>
#include <vector>
#include <boost\shared_ptr.hpp>
namespace AT
{


	class PairExecutor :public IExecutor
	{
	public:
		PairExecutor(const std::string& aConfig);

		virtual ~PairExecutor(void);

		//输入1 来自于上层的交易信号
		virtual  void AddExecution(ExecutorInput aExecutorInput) override;
		virtual  void Abrot() override;

		//输入2 来自于执行层面
		virtual	 void OnMarketDepth(const AT::MarketData& aMarketDepth) override;
		virtual	 void OnRtnOrder(const  AT::OrderUpdate& apOrder)override;
		virtual	 void OnRtnTrade(const  AT::TradeUpdate& apTrade)override;

		virtual ExecutionStatus	GetExecutionStatus()override;
		virtual std::string GetExecutorID()  override;

	private:

		void	ReceivOpenResult(ExecutionResult aResult);
		void	ReceiveCloseResult(ExecutionResult aResult);
		ExecutorInput BuildCloseInput(ExecutionResult aOpenResult);
		void CheckFinished();

		std::string		m_OpenExecutorType;
		std::string		m_OpenExecutorConfig;
		std::string		m_CloseExecutorType;
		std::string		m_CloseExecutorConfig;

		std::string		m_CareInstrumentID;

		boost::shared_ptr<IExecutor>		m_OpenExecutor;

		//每一笔成交都复制一个新的CloseExecutor出来，这样可以避免CloseExecutor的复杂内部状态
		//每一个Executor只处理一笔结果
		std::vector<boost::shared_ptr<IExecutor>> m_CloseExecutorGroup;

		ExecutionStatus m_Status;
		MarketData		m_LastMarketData;
	};

}