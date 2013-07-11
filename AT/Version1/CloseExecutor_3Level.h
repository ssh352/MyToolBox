#pragma once
#include "IExecutor.h"
#include "IDriver_TD.h"
#include <set>
#if 0
namespace AT
{

struct CloseSetting_3Level
{
	int StopLossOrderPrice; //优先单止损价格差
	int QuitLevel_0;
	int EnterLevel_1;
	int QuitLevel_1;
	int EnterLevel_2;
	int QuitLevel_2;
	int EnterLevel_3;
	int QuitLevel_3;
	AT_Time StopTime;//规定时间点还有持仓，则全部平仓
	AT_Time StopClearTime;//规定时间点还有持仓，则按市价清仓
};

class CloseExecutor_3Level :public IExecutor
{
public:
	CloseExecutor_3Level(const std::string& aConfigFile);
	CloseExecutor_3Level(CloseSetting_3Level);
	virtual ~CloseExecutor_3Level(void);

	////输入1 来自于上层的交易信号
	//virtual Command SetupTarget(int targetQuantity,bool isBuy, const AT::MarketData& aMarket) override;
	//virtual Command AddTarget(int addTargetQuantity, bool isBuy,const AT::MarketData& aMarket) override;

	//输入2 来自于执行层面
	virtual  Command OnMarketDepth(const AT::MarketData& aMarketDepth) override;
	virtual  Command OnRtnOrder(const  AT::OrderUpdate& apOrder)override;
	virtual  Command OnRtnTrade(const  AT::TradeUpdate& apTrade)override;
	virtual std::string GetExecutorID() override;



private:
	//止盈止损单(后续跟进优先单)
	Command PlaceStopLossOrder(const AT::MarketData& aMarket);
	//平仓单
	Command PlaceCloseOrder(const AT::MarketData& aMarket);

	Command DoQuit(const AT::MarketData& aMarket);

	int			m_CurrentLevel;
	int			m_StartPrice;
	int			m_PriceCheck;
	int			m_TargetVol;
	int			m_ActiveOrderVol;
	CloseSetting_3Level	m_Setting;
	AT_Order_Key				m_DelOrderSet;
	AT::MarketData m_LastMarket;
	int				m_LastLossPrice;
	AT_Time			m_TriggerLossTime;

private:
	bool		m_IsBuy;
	std::set<AT_Order_Key>		m_SendOrderSet;
	std::set<AT_Order_Key>		m_SendLossSet;

	enum class Status
	{
		IDLE,
		WaitForStopLossOrderPlace,
		WaitQuitSignal,
		WaitForStopLossOrderCancel,
		WaitForNewQuitOrder,
	};
	Status					m_Status;
};

}

#endif