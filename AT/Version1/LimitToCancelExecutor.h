#pragma once
#include "ExecutorBase.h"
#include <set>
#include <string>
namespace AT
{

class LimitToCancelExecutor :public ExecutorBase
{
public:
	LimitToCancelExecutor(const boost::property_tree::ptree& aConfigPtee);

	virtual ~LimitToCancelExecutor(void);

	//����1 �������ϲ�Ľ����ź�
	virtual void DoAddExecution(ExecutorInput aExecutorInput) override;
	virtual void DoAbrot() override;

	//����2 ������ִ�в���
	virtual	void DoOnMarketDepth(const AT::MarketData& aMarketDepth) override;
	virtual	void DoOnRtnOrder(const  AT::OrderUpdate& apOrder)override;

	virtual	void DoOnRtnTrade(const  AT::TradeUpdate& apTrade)override;

	virtual ExecutionStatus GetExecutionStatus() override ;
private:
	void InitFromConfigFile(const std::string& aConfigFile);

	boost::shared_ptr<IExecutor>	m_pLimitExecutor;	
	int								m_CancelTimeVol;
	AT_Time							m_EndTime;

	enum class LimitToCancelStstus
	{
		BeforeBegin,
		Limit_OrderTime,
		PendingCancel,
		Finish
	};

	LimitToCancelStstus			m_Status;
};

}