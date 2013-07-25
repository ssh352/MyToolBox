#pragma once
#include "ExecutorBase.h"
namespace AT
{

class LimitFollowCancelExecutor :public ExecutorBase
{
public:
	LimitFollowCancelExecutor(const boost::property_tree::ptree& aConfigPtee);
	virtual ~LimitFollowCancelExecutor(void);

	virtual	void DoAddExecution(ExecutorInput aExecutorInput) override;
	virtual void DoAbrot() override;	
	virtual	void DoOnMarketDepth(const AT::MarketData& aMarketDepth)override;
	virtual void DoOnRtnOrder(const  AT::OrderUpdate& apOrder)override ;
	virtual void DoOnRtnTrade(const  AT::TradeUpdate& apTrade)override;


private:
	int	m_Limit2FollowTime;
	int m_Follow2CancelTime;
	ExecutorInput BuildFollowExecution();	

	enum  class ExecutorStatus
	{
		IDLE,
		LimitStatus,
		LimitAbortStatus,
		LimitChangeToFollowStatus,
		FollowSatus,
		Finishe
	};

	ExecutorStatus m_Status;

	boost::shared_ptr<IExecutor>	m_pLimitExecutor;
	boost::shared_ptr<IExecutor>	m_pFellowExecutor;

	AT_Time							m_StartTime;

	int					m_TradeQuantity;
	ExecutorInput		m_ExecutorInput;

	void	HandleFirstExecutorResult(ExecutionResult aTrade);
};

}
