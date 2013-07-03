#include "TradeSignalProducerDemo1.h"
#include "IndexContainer.h"
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

	std::string aIndexName;
	int ExpectVal;
	int CheckTimeFromNow;
	int ExecpetTimes;
	bool InvertCheck = false;
	IndexContainer* lpIndexContainer = m_pIndexContainer;
	boost::function<bool(AT_Time aTriggerTime)> lCheckSignal =
		[InvertCheck,lpIndexContainer,aIndexName ,ExpectVal, CheckTimeFromNow, ExecpetTimes](AT_Time aTriggerTime)
	{
		AT_Time lStartTime = aTriggerTime - boost::posix_time::seconds(CheckTimeFromNow);
		int IndexContainer = lpIndexContainer->GetIndexCount(aIndexName.c_str(),ExpectVal,lStartTime,aTriggerTime);
		bool lCheck =  IndexContainer == ExecpetTimes;
		if(InvertCheck)
		{
			return !lCheck;
		}
		else
		{
			return lCheck;
		}
	};
}
void	TradeSignalProducerDemo1::InitConfig(const std::string& aConfigFile)
{
	boost::property_tree::ptree pt;
	read_xml(aConfigFile,pt);
	strSignal signal;
	for( std::pair<std::string,boost::property_tree::ptree>  lSingleName : pt.get_child("SignalConfig.Signals"))
	{
		signal.SignalName = lSingleName.second.get<std::string>("SignalID");
		signal.BuyOrSell = lSingleName.second.get<int>("BuyOrSell")== 1 ?true:false;
		signal.Priority = lSingleName.second.get<int>("Priority");
		signal.HKY006IsUse = lSingleName.second.get<int>("HKY006.IsUse");
		signal.HKY006ItemName = lSingleName.second.get<std::string>("HKY006.ItemName");
		signal.HKY006ReturnValue = lSingleName.second.get<int>("HKY006.ReturnValue");
		m_Signal.push_back(signal);
	}
}
AT::TradeSignal TradeSignalProducerDemo1::ProduceTradeSignal( const MarketData& aTriggerMarket )
{
		TradeSignal lret ;

		int iIndex06 =0;
		for (auto signal:m_Signal)
		{
			if(signal.HKY006IsUse)
			{
				iIndex06 = m_pIndexContainer->GetIndexCount(signal.HKY006ItemName,signal.HKY006ReturnValue,aTriggerMarket.m_UpdateTime-boost::posix_time::minutes(5),aTriggerMarket.m_UpdateTime);
			}
			
			if(iIndex06 >= 1)
			{
				lret.m_Valid = true;
				strcpy_s(lret.m_ID,g_TradeSignalIDLength,signal.SignalName.c_str());
				lret.m_BuyOrSell = signal.BuyOrSell;
				lret.m_priority = signal.Priority;
				lret.m_TriggerMarketData = aTriggerMarket;
				lret.m_TradeSignalSequence = m_Seqence;
				lret.m_TradeSignalType = m_Seqence++;
				WriteTradeSignal(lret);
				return lret;
			}
		}

		lret.m_Valid = false;
		return lret;
}
void    TradeSignalProducerDemo1::WriteTradeSignal(TradeSignal signal)
{
	boost::property_tree::ptree lSignalTree;
	std::string SignalPath = "TradeSignal"+to_iso_string(boost::gregorian::day_clock::local_day());
	std::string BuyOrSell = signal.m_BuyOrSell ?"Buy":"Sell";
	std::string Signal = boost::str(boost::format("TradeSignal [ID:: %s] \n [BuyOrSell: %s] \n  [Priority:: %d] \n [InstrumentID:%s]\n[Price: %d] \n [Time: %s] \n") 
										   % signal.m_ID
										   % BuyOrSell
										   % signal.m_priority
										   % signal.m_TriggerMarketData.InstrumentID
										   % signal.m_TriggerMarketData.m_LastPrice
										   % ToString(signal.m_TriggerMarketData.m_UpdateTime)
										   );
	lSignalTree.add("TradeSignal:",Signal);
	write_xml(SignalPath,lSignalTree);
}
bool TradeSignalProducerDemo1::CheckSignal(AT_Time aTriggerTime)
{
	std::string aIndexName;
	int ExpectVal;
	int CheckTimeFromNow;
	int ExecpetTimes;
	bool InvertCheck;
	IndexContainer* lpIndexContainer = m_pIndexContainer;
	AT_Time lStartTime = aTriggerTime - boost::posix_time::seconds(CheckTimeFromNow);
	int IndexContainer = lpIndexContainer->GetIndexCount(aIndexName.c_str(),ExpectVal,lStartTime,aTriggerTime);
	bool lCheck =  IndexContainer == ExecpetTimes;
	if(InvertCheck)
	{
		return !lCheck;
	}
	else
	{
		return lCheck;
	}
}
}
