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
	OpenLimitExecutor(int ValidTimeInSecond,int iBufferPoint);
	virtual ~OpenLimitExecutor(void);

	//输入1 来自于上层的交易信号
	virtual boost::shared_ptr<TradeCommand> SetupTarget(int targetQuantity,bool isBuy, const AT::MarketData& aMarket) override;
	virtual boost::shared_ptr<TradeCommand> AddTarget(int addTargetQuantity, bool isBuy,const AT::MarketData& aMarket) override;

	//输入2 来自于执行层面
	virtual  boost::shared_ptr<TradeCommand> OnMarketDepth(const AT::MarketData& aMarketDepth) override;
	virtual  boost::shared_ptr<TradeCommand> OnRtnOrder(const  AT::OrderUpdate& apOrder)override;
	virtual  boost::shared_ptr<TradeCommand> OnRtnTrade(const  AT::TradeUpdate& apTrade)override;
	virtual std::string GetExecutorID()  override;
	boost::shared_ptr<TradeCommand> PlaceOrder(int addTargetQuantity,int Price); 
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