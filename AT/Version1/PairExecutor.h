#pragma once
#include "IExecutor.h"
#include <memory>
#include <vector>
#include <boost\shared_ptr.hpp>
#include <boost\property_tree\ptree.hpp>
namespace AT
{


	class PairExecutor :public IExecutor
	{
	public:
		PairExecutor(const boost::property_tree::ptree& aConfigPtee);

		virtual ~PairExecutor(void);

		//输入1 来自于上层的交易信号
		virtual  void AddExecution(ExecutorInput aExecutorInput) override;
		virtual  void Abrot() override;

		//输入2 来自于执行层面
		virtual	 void OnMarketDepth(const AT::MarketData& aMarketDepth) override;
		virtual	 void OnRtnOrder(const  AT::OrderUpdate& apOrder)override;
		virtual	 void OnRtnTrade(const  AT::TradeUpdate& apTrade)override;



		//输入3 设置完成任务的callback
		virtual void SetTradeReportCallback (TradeReportFun aFinishCallback)  {m_TradeReport = aFinishCallback;} ;
	
		//输入4 设置一个接受Command的回掉，因为有可能一个更新要发出多个Command
		//比如开仓成功的同时要下一个平仓单，同时开仓部分自己也可能会有单子。
		virtual void SetCommandHandler(CommandHandler aCommandHandler)
		{
			m_CommandHandle = aCommandHandler;
		}

		virtual ExecutionStatus	GetExecutionStatus() override;
		virtual std::string GetExecutorID()  override;

	private:
		CommandHandler				m_CommandHandle;
		TradeReportFun				m_TradeReport;
		std::string					m_ExecutorID;

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