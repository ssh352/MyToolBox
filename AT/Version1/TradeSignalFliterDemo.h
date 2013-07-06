#pragma once
#include "ITradeSignalFliter.h"
namespace AT
{

	struct FliterStruct
	{
		int		Time1;
		int		Time2;
		int		Time3;
		std::map<int,int> TotalTimeProfit;
		int		TotalProfitStop;
		AT_Time StartTime;
		AT_Time StopTime;
	};

class TradeSignalFliterDemo :public ITradeSignalFliter
{
public:
	TradeSignalFliterDemo(void);
	virtual ~TradeSignalFliterDemo(void);

public:
	virtual Signal FliterTradeSignal(std::vector<Signal> aList) override;

	virtual void OnMarketDepth(const AT::MarketData&  aMarket) override;
	virtual void UpdateProfit(int32_t aProfit,AT_Time aTime) override ;
	virtual void OnAccountCompleteSignal() override;

private:
	struct LastNTradeStatus
	{
		bool m_isAllLoss;
		int	m_TotalLoss;
		AT_Time m_TimeLoss;
	};
	LastNTradeStatus IsLastNTradeLoss(int i);


	bool m_IsOnLastSignal;
	AT_Time m_LastTime;
	std::map<AT_Time,int32_t> m_ProfitStatusMap;

private:

	int m_StopTriggerVol2;
	int m_StopTriggerVol3;

	std::map<int,FliterStruct>	m_FliterStructMap;
};

}