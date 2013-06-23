#pragma once
#include "ITradeSignalFliter.h"
namespace AT
{


class TradeSignalFliterDemo :public ITradeSignalFliter
{
public:
	TradeSignalFliterDemo(void);
	virtual ~TradeSignalFliterDemo(void);

public:
	virtual TradeSignal FliterTradeSignal(std::vector<TradeSignal> aList) override;

	virtual void OnMarketDepth(const AT::MarketData&  aMarket) override;
	virtual void UpdateProfit(int32_t aProfit,AT_Time aTime) override ;

private:
	struct LastNTradeStatus
	{
		bool m_isAllLoss;
		int	m_TotalLoss;
	};
	LastNTradeStatus IsLastNTradeLoss(int i);


	bool m_IsOnLastSignal;
	AT_Time m_LastTime;
	AT_Time	m_StopTime;
	std::map<AT_Time,int32_t> m_ProfitStatusMap;

private:

	int m_StopTriggerVol2;
	int m_StopTriggerVol3;

	int  m_Time1;
	int	 m_Time2;
	int  m_Time3;
	int  m_TotalProfitStopVal;
	std::map<int ,int> m_StopLevelTimeMap;
};

}