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

		//����1 �������ϲ�Ľ����ź�
		virtual  void AddExecution(ExecutorInput aExecutorInput) override;
		virtual  void Abrot() override;

		//����2 ������ִ�в���
		virtual	 void OnMarketDepth(const AT::MarketData& aMarketDepth) override;
		virtual	 void OnRtnOrder(const  AT::OrderUpdate& apOrder)override;
		virtual	 void OnRtnTrade(const  AT::TradeUpdate& apTrade)override;



		//����3 ������������callback
		virtual void SetTradeReportCallback (TradeReportFun aFinishCallback)  {m_TradeReport = aFinishCallback;} ;
	
		//����4 ����һ������Command�Ļص�����Ϊ�п���һ������Ҫ�������Command
		//���翪�ֳɹ���ͬʱҪ��һ��ƽ�ֵ���ͬʱ���ֲ����Լ�Ҳ���ܻ��е��ӡ�
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

		//ÿһ�ʳɽ�������һ���µ�CloseExecutor�������������Ա���CloseExecutor�ĸ����ڲ�״̬
		//ÿһ��Executorֻ����һ�ʽ��
		std::vector<boost::shared_ptr<IExecutor>> m_CloseExecutorGroup;

		ExecutionStatus m_Status;
		MarketData		m_LastMarketData;
	};

}