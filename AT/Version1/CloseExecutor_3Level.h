#pragma once
#include "ITradeSignalExecutor.h"
#include "IDriver_TD.h"
#include <set>
namespace AT
{

struct CloseSetting_3Level
{
	int QuitLevel_0;
	int EnterLevel_1;
	int QuitLevel_1;
	int EnterLevel_2;
	int QuitLevel_2;
};

class CloseExecutor_3Level :public ITradeSignalExecutor
{
public:
	CloseExecutor_3Level(CloseSetting_3Level);
	virtual ~CloseExecutor_3Level(void);

	//输入1 来自于上层的交易信号
	virtual boost::shared_ptr<TradeCommand> SetupTarget(int targetQuantity,bool isBuy, const AT::MarketData& aMarket) override;
	virtual boost::shared_ptr<TradeCommand> AddTarget(int addTargetQuantity, bool isBuy,const AT::MarketData& aMarket) override;

	//输入2 来自于执行层面
	virtual  boost::shared_ptr<TradeCommand> OnMarketDepth(const AT::MarketData& aMarketDepth) override;
	virtual  boost::shared_ptr<TradeCommand> OnRtnOrder(const  AT::OrderUpdate& apOrder)override;
	virtual  boost::shared_ptr<TradeCommand> OnRtnTrade(const  AT::TradeUpdate& apTrade)override;

	virtual void SetFinishedCallbakc(FinishExecuteCallbackType aFinishCallback);

	virtual std::string GetExecutorID() override;

private:
	boost::shared_ptr<TradeCommand> DoQuit(const AT::MarketData& aMarket);

	int			m_CurrentLevel;
	int			m_StartPrice;
	int			m_PriceCheck;
	int			m_TargetVol;
	int			m_ActiveOrderVol;
	CloseSetting_3Level	m_Setting;

private:
	bool		m_IsBuy;
	FinishExecuteCallbackType  m_FinishehNotfiy;
	std::set<AT_Order_Key>		m_SendOrderSet;
};

}