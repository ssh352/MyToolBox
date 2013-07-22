#include "FilterSystemLevel.h"
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>
#include "boost/date_time/posix_time/posix_time.hpp"
#include "boost/date_time/posix_time/posix_time_types.hpp"
#include <boost/format.hpp>
#include "ATLogger.h"
namespace AT
{


FilterSystemLevel::FilterSystemLevel(void)
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


FilterSystemLevel::~FilterSystemLevel(void)
{
}

Signal FilterSystemLevel::FliterTradeSignal( std::vector<Signal> aList )
{
	if(aList.size() == 0)
	{
		Signal lret;
		lret.m_Valid = false;
		return lret;
	}
	///////////据信号id产生过滤信号id///////
	int iFliterID = 1;

	if(m_IsOnLastSignal)
	{
		Signal lret;
		lret.m_Valid = false;
		return lret;
	}
	//开始开仓时间
	if (m_LastTime < m_FliterStructMap[iFliterID].StartTime )
	{
		Signal lret;
		lret.m_Valid = false;
		return lret;
	}
	//停止开仓时间
	if(m_LastTime > m_FliterStructMap[iFliterID].StopTime)
	{
		Signal lret;
		lret.m_Valid = false;
		return lret;
	}
	
}


void FilterSystemLevel::UpdateProfit( int32_t aProfit,AT_Time aTime )
{
	m_ProfitStatusMap[aTime] = aProfit;
	m_IsOnLastSignal = false;
}

void FilterSystemLevel::OnMarketDepth( const AT::MarketData& aMarket )
{
	m_LastTime  = aMarket.m_UpdateTime;
}

FilterSystemLevel::LastNTradeStatus FilterSystemLevel::IsLastNTradeLoss( int lastI )
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

void FilterSystemLevel::OnAccountCompleteSignal()
{

}



}