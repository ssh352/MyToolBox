#pragma once
#include "ITradeSignalExecutor.h"
#include <set>
namespace AT
{


class OpenLimitExecutor :public ITradeSignalExecutor
{
public:
	OpenLimitExecutor(int ValidTimeInSecond);
	virtual ~OpenLimitExecutor(void);

	//����1 �������ϲ�Ľ����ź�
	virtual boost::shared_ptr<TradeCommand> SetupTarget(int targetQuantity,bool isBuy, const AT::MarketData& aMarket) override;
	virtual boost::shared_ptr<TradeCommand> AddTarget(int addTargetQuantity, bool isBuy,const AT::MarketData& aMarket) override;

	//����2 ������ִ�в���
	virtual  boost::shared_ptr<TradeCommand> OnMarketDepth(const AT::MarketData& aMarketDepth) override;
	virtual  boost::shared_ptr<TradeCommand> OnRtnOrder(const  AT::OrderUpdate& apOrder)override;
	virtual  boost::shared_ptr<TradeCommand> OnRtnTrade(const  AT::TradeUpdate& apTrade)override;
	virtual std::string GetExecutorID()  override;

	boost::shared_ptr<TradeCommand> AddOrder(int vol);

private:
	std::set<AT_Order_Key>		m_SendOrderSet;
	AT_Time						m_StartTime;
	int							m_TragetVol;
	AT::MarketData				m_LastMarket;
	bool						m_IsBuy;
	int							m_OrderValidTime;
	OrderType					m_OrderType;
	AT_Order_Key				m_DelOrderSet;
};

}