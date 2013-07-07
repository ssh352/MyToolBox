#pragma once
#include "IExecutor.h"
#include <set>
#include <string>
namespace AT
{


class OpenLimitExecutor :public IExecutor
{
public:
	OpenLimitExecutor(const std::string& aConfigFile);
	virtual ~OpenLimitExecutor(void);

	//输入1 来自于上层的交易信号
	virtual  Command AddExecution(ExecutorInput aExecutorInput) override;
	//输入2 来自于执行层面
	virtual  Command OnMarketDepth(const AT::MarketData& aMarketDepth) override;
	virtual  Command OnRtnOrder(const  AT::OrderUpdate& apOrder)override;
	virtual  Command OnRtnTrade(const  AT::TradeUpdate& apTrade)override;
	virtual std::string GetExecutorID()  override;
	Command PlaceOrder(int addTargetQuantity,int Price); 


private:
	void InitFromConfigFile(const std::string& aConfigFile);

private:
	std::set<AT_Order_Key>		m_SendOrderSet;
	AT_Time						m_StartTime;
	int							m_TragetVol;
	AT::MarketData				m_LastMarket;
	bool						m_IsBuy;
	int							m_OrderValidTime;
	AT_Order_Key				m_DelOrderSet;
	int							m_iBufferPoint;//缓冲点
	std::string					m_InstrumentID;
};

}