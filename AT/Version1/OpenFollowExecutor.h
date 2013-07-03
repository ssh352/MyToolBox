#pragma once
#include "ITradeSignalExecutor.h"
#include <set>
namespace AT
{


	struct  FollowExecutorParma
	{
		int m_FollowTime;
		int	m_FollowRange;
	};

class OpenFollowExecutor :public ITradeSignalExecutor
{
public:
	OpenFollowExecutor(FollowExecutorParma);
	virtual ~OpenFollowExecutor(void);

	//����1 �������ϲ�Ľ����ź�
	virtual boost::shared_ptr<TradeCommand> SetupTarget(int targetQuantity,bool isBuy, const AT::MarketData& aMarket) override;
	virtual boost::shared_ptr<TradeCommand> AddTarget(int addTargetQuantity, bool isBuy,const AT::MarketData& aMarket) override;

	

	//����2 ������ִ�в���
	virtual  boost::shared_ptr<TradeCommand> OnMarketDepth(const AT::MarketData& aMarketDepth) override;
	virtual  boost::shared_ptr<TradeCommand> OnRtnOrder(const  AT::OrderUpdate& apOrder)override;
	virtual  boost::shared_ptr<TradeCommand> OnRtnTrade(const  AT::TradeUpdate& apTrade)override;
	virtual std::string GetExecutorID()  override;

private:
	boost::shared_ptr<TradeCommand> PlaceOrder(int addTargetQuantity) ;
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