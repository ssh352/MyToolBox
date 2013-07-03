#pragma once
#include "ITradeSignalExecutor.h"
#include <set>
namespace AT
{


class OpenMarketExecutor :public ITradeSignalExecutor
{
public:
	OpenMarketExecutor();
	virtual ~OpenMarketExecutor(void);

	//输入1 来自于上层的交易信号
	virtual boost::shared_ptr<TradeCommand> SetupTarget(int targetQuantity,bool isBuy, const AT::MarketData& aMarket) override;
	virtual boost::shared_ptr<TradeCommand> AddTarget(int addTargetQuantity, bool isBuy,const AT::MarketData& aMarket) override;

	//输入2 来自于执行层面
	virtual  boost::shared_ptr<TradeCommand> OnMarketDepth(const AT::MarketData& aMarketDepth) override;
	virtual  boost::shared_ptr<TradeCommand> OnRtnOrder(const  AT::OrderUpdate& apOrder)override;
	virtual  boost::shared_ptr<TradeCommand> OnRtnTrade(const  AT::TradeUpdate& apTrade)override;
	virtual std::string GetExecutorID()  override;

private:
	int							m_TragetVol;
	bool						m_IsBuy;
	std::set<AT_Order_Key>		m_SendOrderSet;
};

}