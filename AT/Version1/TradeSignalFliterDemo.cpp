#include "TradeSignalFliterDemo.h"
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>
#include "boost/date_time/posix_time/posix_time.hpp"
#include "boost/date_time/posix_time/posix_time_types.hpp"
#include <boost/format.hpp>

namespace AT
{


TradeSignalFliterDemo::TradeSignalFliterDemo(void)
{
	//load from  files
	boost::property_tree::ptree lpt;
	read_xml("SignalFliter.xml",lpt);

	FliterStruct fliter;
	int iID;
	int iTime;
	std::string strStopTime;
	std::string strStartTime;
	for (auto lSignalFliter:lpt.get_child("SignalFliter"))
	{
		iID = lSignalFliter.second.get<int>("ID");
		fliter.Time1 = lSignalFliter.second.get<int>("Time1");
		fliter.Time2 = lSignalFliter.second.get<int>("Time2");
		fliter.Time3 = lSignalFliter.second.get<int>("Time3");
		iTime = lSignalFliter.second.get<int>("TotalTimeProfit.Time");
		fliter.TotalTimeProfit[iTime] = lSignalFliter.second.get<int>("TotalTimeProfit.Profit");
		fliter.TotalProfitStop = lSignalFliter.second.get<int>("TotalProfitStop");
		strStopTime = to_simple_string(boost::gregorian::day_clock::local_day())+" "+lpt.get<std::string>("SignalFliter.Item.StopTime");
		fliter.StopTime = boost::posix_time::time_from_string(strStopTime);
		strStartTime = to_simple_string(boost::gregorian::day_clock::local_day())+" "+lpt.get<std::string>("SignalFliter.Item.StartTime");
		fliter.StartTime = boost::posix_time::time_from_string(strStartTime);
		m_FliterStructMap[iID] = fliter;
	}
	m_StopTriggerVol2;
	m_StopTriggerVol3;

	m_IsOnLastSignal = false;
}


TradeSignalFliterDemo::~TradeSignalFliterDemo(void)
{
}

TradeSignal TradeSignalFliterDemo::FliterTradeSignal( std::vector<TradeSignal> aList )
{
	if(aList.size() == 0)
	{
		TradeSignal lret;
		lret.m_Valid = false;
		return lret;
	}
	///////////据信号id产生过滤信号id///////
	int iFliterID = 1;

	if(m_IsOnLastSignal)
	{
		TradeSignal lret;
		lret.m_Valid = false;
		return lret;
	}
	//开始开仓时间
	if (m_LastTime < m_FliterStructMap[iFliterID].StartTime )
	{
		TradeSignal lret;
		lret.m_Valid = false;
		return lret;
	}
	//停止开仓时间
	if(m_LastTime > m_FliterStructMap[iFliterID].StopTime)
	{
		TradeSignal lret;
		lret.m_Valid = false;
		return lret;
	}

	//连续亏损1次

	LastNTradeStatus lret1 = IsLastNTradeLoss(1);

	if(lret1.m_isAllLoss && (m_LastTime - m_ProfitStatusMap.rbegin()->first) < boost::posix_time::seconds(m_FliterStructMap[iFliterID].Time1) )
	{
		TradeSignal lret;
		lret.m_Valid = false;
		return lret;
	}
	//2次

	LastNTradeStatus lret2 = IsLastNTradeLoss(2);
//	if(lret2.m_isAllLoss && lret2.m_TotalLoss > m_StopTriggerVol2 )
	if(lret2.m_isAllLoss && (m_LastTime-lret2.m_TimeLoss)<boost::posix_time::seconds(m_FliterStructMap[iFliterID].Time2))
	{
		TradeSignal lret;
		lret.m_Valid = false;
		return lret;
	}


	LastNTradeStatus lret3 = IsLastNTradeLoss(3);
	//if(lret3.m_isAllLoss && lret3.m_TotalLoss > m_StopTriggerVol3 )
	if(lret3.m_isAllLoss && (m_LastTime-lret3.m_TimeLoss)<boost::posix_time::seconds(m_FliterStructMap[iFliterID].Time3))
	{
		TradeSignal lret;
		lret.m_Valid = false;
		return lret;
	}
	//3次

	//总亏损
	int totalProfit = 0;
	for (auto lProfitPair:m_ProfitStatusMap)
	{
		totalProfit += lProfitPair.second;
	}
	if(totalProfit < m_FliterStructMap[iFliterID].TotalProfitStop)
	{
		TradeSignal lret;
		lret.m_Valid = false;
		return lret;
	}

	//总亏损N点时N秒冻结时长
	if(totalProfit < m_FliterStructMap[iFliterID].TotalTimeProfit.begin()->first &&
		(m_LastTime - m_ProfitStatusMap.rbegin()->first) < boost::posix_time::seconds(m_FliterStructMap[iFliterID].TotalTimeProfit.begin()->second))
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

TradeSignalFliterDemo::LastNTradeStatus TradeSignalFliterDemo::IsLastNTradeLoss( int lastI )
{
	LastNTradeStatus lret = {false,0};
	std::map<AT_Time,int32_t>::reverse_iterator iter = m_ProfitStatusMap.rbegin();
	for (int i = 0; i< lastI;++i)
	{
		if(iter!= m_ProfitStatusMap.rend() && iter->second < 0 )
		{
			lret.m_TotalLoss += iter->second;
			lret.m_TimeLoss = iter->first;
		}
		else
		{
			lret.m_isAllLoss = false;
			return lret;
		}
		iter--;
	}
	lret.m_isAllLoss = true;
	return lret;
}



}