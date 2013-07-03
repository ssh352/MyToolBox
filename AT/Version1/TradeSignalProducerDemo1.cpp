#include "TradeSignalProducerDemo1.h"
#include "IndexContainer.h"
#include <ATLogger.h>
#include "boost/format.hpp"
#include "boost/format/free_funcs.hpp"
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>
#include "boost/function.hpp"
namespace AT
{


TradeSignalProducerDemo1::~TradeSignalProducerDemo1(void)
{
}

TradeSignalProducerDemo1::TradeSignalProducerDemo1(const std::string& aConfigFile, IndexContainer* apIndexContainer )
	:m_pIndexContainer(apIndexContainer)
	,m_Seqence(0)
{
	InitConfig(aConfigFile);
}
void TradeSignalProducerDemo1::InitConfig(const std::string& aConfigFile)
{
	boost::property_tree::ptree pt;
	read_xml(aConfigFile,pt);
	m_SignalName = pt.get<std::string>("SignalConfig.SignalID");
	m_IsBuy = pt.get<bool>("SignalConfig.IsBuy");
	m_Priority = pt.get<int32_t>("SignalConfig.Priority");

	for( std::pair<std::string,boost::property_tree::ptree>  lChecklist : pt.get_child("SignalConfig.CheckItems"))
	{


		std::string aIndexName = lChecklist.second.get<std::string>("IndexID");
		int lCheckType =  lChecklist.second.get<int>("CheckType");
		IndexContainer* lpIndexContainer = m_pIndexContainer;
		switch (lCheckType)
		{
		case CountCheckType:
			{
				int ExpectVal = lChecklist.second.get<int>("ExceptVal");
				int CheckTimeFromNow = lChecklist.second.get<int>("TimeFromNow");
				int ExecpetTimes  = lChecklist.second.get<int>("OccurTimes");
				bool InvertCheck =  lChecklist.second.get<bool>("IsBlock");
				boost::function<bool(AT_Time aTriggerTime)> lCheckSignal =
					[InvertCheck,lpIndexContainer,aIndexName ,ExpectVal, CheckTimeFromNow, ExecpetTimes](AT_Time aTriggerTime)
				{
					AT_Time lStartTime = aTriggerTime - boost::posix_time::seconds(CheckTimeFromNow);
					int IndexContainer = lpIndexContainer->GetIndexCount(aIndexName.c_str(),ExpectVal,lStartTime,aTriggerTime);
					bool lCheck =  IndexContainer == ExecpetTimes;
					if(InvertCheck)
					{
						return ! lCheck;
					}
					else
					{
						return lCheck;
					}
				};
				m_CheckList.push_back(lCheckSignal);
			}
			break;
		case LastValCheckType:
			{
				bool InvertCheck =  lChecklist.second.get<bool>("IsBlock");
				int ExpectVal = lChecklist.second.get<int>("ExceptVal");

				boost::function<bool(AT_Time aTriggerTime)> lCheckSignal =
					[InvertCheck,lpIndexContainer,aIndexName ,ExpectVal](AT_Time aTriggerTime)
				{
					int lLastVal = lpIndexContainer->GetIndex(aIndexName.c_str());
					bool lCheck =  lLastVal == ExpectVal;
					if(InvertCheck)
					{
						return ! lCheck;
					}
					else
					{
						return lCheck;
					}
				};
				m_CheckList.push_back(lCheckSignal);
			}
			break;
		case LastNonZeroCheckType:
			break;
		default:
			break;
		}
	
	}


}
AT::TradeSignal TradeSignalProducerDemo1::ProduceTradeSignal( const MarketData& aTriggerMarket )
{
		TradeSignal lret ;
		lret.m_BuyOrSell = m_IsBuy;
		lret.m_priority = m_Priority;
		strcpy_s(lret.m_ID ,g_TradeSignalIDLength, m_SignalName.c_str());


		bool lPassAllCheck = true;
		for (auto lCheckFun:m_CheckList)
		{
			lPassAllCheck &= lCheckFun(aTriggerMarket.m_UpdateTime);
		}
		
		if(lPassAllCheck)
		{
			lret.m_Valid  = true;
			lret.m_TradeSignalSequence = m_Seqence++;
			lret.m_TriggerMarketData = aTriggerMarket;
			ATLOG(AT::LogLevel::L_INFO,"xxxxxxxxxx Trigger TradeSignal");
		}
		else
		{
			lret.m_Valid = false;
		}
		return lret;
}

}
