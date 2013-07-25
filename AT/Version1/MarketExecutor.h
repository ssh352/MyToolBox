#pragma once
#include "ExecutorBase.h"
#include <set>
#include <string>
#include <map>
namespace AT
{


class MarketExecutor :public ExecutorBase
{
public:
	MarketExecutor(const boost::property_tree::ptree& aConfigPtee);

	virtual ~MarketExecutor(void);

	//����1 �������ϲ�Ľ����ź�
	virtual void DoAddExecution(ExecutorInput aExecutorInput) override;
	virtual void DoAbrot() override;

	//����2 ������ִ�в���
	virtual	void DoOnMarketDepth(const AT::MarketData& aMarketDepth) override;
	virtual	void DoOnRtnOrder(const  AT::OrderUpdate& apOrder)override;
	virtual	void DoOnRtnTrade(const  AT::TradeUpdate& apTrade)override;

private:
	
	Command			BuildCommand(ExecutorInput aNewOrder);
	
	
};

}