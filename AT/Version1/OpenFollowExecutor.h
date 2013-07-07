#pragma once
#include "IExecutor.h"
#include <set>
namespace AT
{


	struct  FollowExecutorParma
	{
		int m_FollowTime;
		int	m_FollowRange;
	};

class OpenFollowExecutor :public IExecutor
{
public:
	OpenFollowExecutor(const std::string& configFile);
	OpenFollowExecutor(FollowExecutorParma);
	virtual ~OpenFollowExecutor(void);

	//输入1 来自于上层的交易信号
	virtual  Command AddExecution(ExecutorInput aExecutorInput) override;

	

	//输入2 来自于执行层面
	virtual  Command OnMarketDepth(const AT::MarketData& aMarketDepth) override;
	virtual  Command OnRtnOrder(const  AT::OrderUpdate& apOrder)override;
	virtual  Command OnRtnTrade(const  AT::TradeUpdate& apTrade)override;
	virtual std::string GetExecutorID()  override;

private:
	Command PlaceOrder(int addTargetQuantity) ;
	bool IsOrderPriceNeedModify(const AT::MarketData& aMarketDepth);

	int							m_TragetVol;
	bool						m_IsBuy;
	std::set<AT_Order_Key>		m_SendOrderSet;
	AT_Order_Key				m_LastSendOrderKey;
	int							m_LastSendOrderPrice;
	FollowExecutorParma			m_Setting;
	AT_Time						m_StartTime;
	std::string					m_InstrumentID;
	int							m_LastPrice;
	int							m_LastOrderPrice;

	enum  class FollowExecutorStatus
	{
		IDLE,
		WaitingOrderPlace,
		OrderPlaced,
		WaitingOrderCancel,
	};
	FollowExecutorStatus	  m_ExecutorStatus;

};

}