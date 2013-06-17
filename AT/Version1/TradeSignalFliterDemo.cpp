#include "TradeSignalFliterDemo.h"

namespace AT
{


TradeSignalFliterDemo::TradeSignalFliterDemo(void)
{
}


TradeSignalFliterDemo::~TradeSignalFliterDemo(void)
{
}

TradeSignal TradeSignalFliterDemo::FliterTradeSignal( std::vector<TradeSignal> aList )
{
	if(m_IsOnLastSignal)
	{
		TradeSignal lret;
		lret.m_Valid = false;
		return lret;
	}

	if(m_LastTime > m_StopTime)
	{
		TradeSignal lret;
		lret.m_Valid = false;
		return lret;
	}

	//连续亏损1次
	if(m_ProfitStatusMap.size() >= 1 && m_ProfitStatusMap.rbegin()->second < 0 && (m_LastTime - m_ProfitStatusMap.rbegin()->first)  > boost::posix_time::seconds(m_Time1) )
	{
		TradeSignal lret;
		lret.m_Valid = false;
		return lret;
	}
	//2次
	//3次

	//总亏损
	int totalProfit = 0;
	for (auto lProfitPair:m_ProfitStatusMap)
	{
		totalProfit += lProfitPair.second;
	}
	if(totalProfit < m_TotalProfitStopVal)
	{
		TradeSignal lret;
		lret.m_Valid = false;
		return lret;
	}


	//处理优先级
	TradeSignal lret;
	lret.m_Valid = false;
	lret.m_priority = -1000;
	for(auto lSignal :aList)
	{
		if(lSignal.m_priority > lret.m_priority && lSignal.m_Valid )
		{
			lret = lSignal;
		}
	}

	m_IsOnLastSignal = true;
	return lret;


}


void TradeSignalFliterDemo::UpdateProfit( int32_t aProfit,AT_Time aTime )
{
	m_ProfitStatusMap[aTime] = aProfit;
	m_IsOnLastSignal = false;
}

void TradeSignalFliterDemo::OnMarketDepth( const AT::MarketData& aMarket )
{
	m_LastTime  = aMarket.m_UpdateTime;
}



}