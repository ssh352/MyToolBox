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

		//����1 �������ϲ�Ľ����ź�
		virtual  void AddExecution(ExecutorInput aExecutorInput) override;
		virtual  void Abrot() override;

		//����2 ������ִ�в���
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

		//ÿһ�ʳɽ�������һ���µ�CloseExecutor�������������Ա���CloseExecutor�ĸ����ڲ�״̬
		//ÿһ��Executorֻ����һ�ʽ��
		std::vector<boost::shared_ptr<IExecutor>> m_CloseExecutorGroup;

		ExecutionStatus m_Status;
		MarketData		m_LastMarketData;
	};

}