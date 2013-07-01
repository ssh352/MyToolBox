#include "TradeSignalProducerDemo1.h"
#include "IndexContainer.h"
#include "boost/format.hpp"
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>
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
void	TradeSignalProducerDemo1::InitConfig(const std::string& aConfigFile)
{
	boost::property_tree::ptree pt;
	read_xml(aConfigFile,pt);
	strSignal signal;
	for( std::pair<std::string,boost::property_tree::ptree>  lSingleName : pt.get_child("SignalConfig.Signals"))
	{
		signal.SignalName = lSingleName.second.get<std::string>("SignalName");
		signal.BuyOrSell = lSingleName.second.get<int>("BuyOrSell")== 1 ?true:false;
		signal.Priority = lSingleName.second.get<int>("Priority");
		signal.HKY006 = lSingleName.second.get<int>("HKY006");
		m_Signal.push_back(signal);
	}
}
AT::TradeSignal TradeSignalProducerDemo1::ProduceTradeSignal( const MarketData& aTriggerMarket )
{
		TradeSignal lret ;
		int iIndex06 = m_pIndexContainer->GetIndex("HKY006",7,aTriggerMarket.m_UpdateTime-boost::posix_time::minutes(1),aTriggerMarket.m_UpdateTime);
		for (auto signal:m_Signal)
		{
			if(signal.HKY006 == iIndex06)
			{
				lret.m_Valid = true;
				strcpy_s(lret.m_ID,g_TradeSignalIDLength,signal.SignalName.c_str());
				lret.m_BuyOrSell = signal.BuyOrSell;
				lret.m_priority = signal.Priority;
				lret.m_TriggerMarketData = aTriggerMarket;
				lret.m_TradeSignalSequence = m_Seqence;
				lret.m_TradeSignalType = m_Seqence++;
				return lret;
			}
		}

		lret.m_Valid = false;
		return lret;
}

}
