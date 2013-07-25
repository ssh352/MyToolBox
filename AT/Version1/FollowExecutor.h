#pragma once
#include "ExecutorBase.h"
#include <set>
#include <string>
#include <map>
namespace AT
{


class FollowExecutor :public ExecutorBase
{
public:
	FollowExecutor(const boost::property_tree::ptree& aConfigPtee);


	virtual ~FollowExecutor(void);

	//输入1 来自于上层的交易信号
	virtual void DoAddExecution(ExecutorInput aExecutorInput) override;
	virtual void DoAbrot() override;
	virtual	void DoOnMarketDepth(const AT::MarketData& aMarketDepth) override;
	virtual	void DoOnRtnOrder(const  AT::OrderUpdate& apOrder)override;
	virtual	void DoOnRtnTrade(const  AT::TradeUpdate& apTrade)override;



private:
	void SetupTradeInfo( const AT::OrderUpdate &aOrder );

	Command			BuildCommand(int vol);
	AT_Order_Key				m_OrderKeyBase;
	AT::OrderUpdate				m_TheOnlyOneMarketOrder;
	ExecutionStatus				m_ExecutionStatusBase;

	enum class FollowStatus
	{
		BeforeStart,
		Running,
		PendingCancel,
		Finished,
	};
	FollowStatus m_Status;

	//
	BuySellType		m_BuySell;
	OpenCloseType	m_OpenClose;
	std::string		m_InstrumentIDBase;
	
};

}