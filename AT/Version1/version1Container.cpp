#include "version1Container.h"
#include "IndexContainer.h"
#include "ITradeSignalProducer.h"
#include "ITradeSignalFliter.h"
#include "ITradeSignalExecutor.h"
#include <boost\bind.hpp>
#include "TradeSignalFliterDemo.h"
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>
#include "ISignalModule.h"
#include <windows.h>
#include <boost\format.hpp>
#include "TradeSignalProducerDemo1.h"
namespace AT
{


version1Container::version1Container(const char* aConfigFile, AT::IDriver_TD* apTD, AT::IStrategySpi* aStrSpi, const  AT::IMarketCache* apMarketCache)
	:m_pTD(apTD)
	,m_pMarketCache(apMarketCache)
	,m_TradeAccountContaner(aConfigFile,apTD)
{
////todo init
//	ITradeSignalFliter*					m_pTradeSignalFliter;
	 InitIndexContainer();
	 InitFliter();
	 InitAccountContainer();
	 InitSignalProducer();
}


version1Container::~version1Container(void)
{
}

void version1Container::OnMarketDepth( const AT::MarketData& aMarketDepth )
{
	UpdateSubPartMarket(aMarketDepth);

	std::vector<TradeSignal> lTradeSignalVec = ProduceTradeSignal(aMarketDepth);

	TradeSignal lFinalSignal = m_pTradeSignalFliter->FliterTradeSignal(lTradeSignalVec);
	if(lFinalSignal.m_Valid)
	{
		m_TradeAccountContaner.HandleTradeSignal(lFinalSignal);
	}


}

std::vector<TradeSignal> version1Container::ProduceTradeSignal( const AT::MarketData& lNow )
{
	std::vector<TradeSignal> lSignalResult;
	TradeSignal tradeSignal;
	for(auto lpSignalProducer :m_TradeSignalProducerVec)
	{
		tradeSignal = lpSignalProducer->ProduceTradeSignal(lNow);
		if(tradeSignal.m_Valid)
		{
			lSignalResult.push_back(lpSignalProducer->ProduceTradeSignal(lNow));
		}
	}
	return lSignalResult;
}

void version1Container::UpdateSubPartMarket( const AT::MarketData& aMarketDepth )
{
	m_pIndexContaner->OnMarketDepth(aMarketDepth);
	m_pTradeSignalFliter->OnMarketDepth(aMarketDepth);
	m_TradeAccountContaner.OnMarketDepth(aMarketDepth);
}

void version1Container::OnRtnOrder( const OrderUpdate& apOrder )
{
	m_TradeAccountContaner.OnRtnOrder(apOrder);
}

void version1Container::OnRtnTrade( const TradeUpdate& apTrade )
{
	m_TradeAccountContaner.OnRtnTrade(apTrade);
}

void version1Container::InitIndexContainer()
{
	m_pIndexContaner =  new IndexContainer("IndexContainer.xml",m_pMarketCache);
}

void version1Container::InitAccountContainer()
{
	m_TradeAccountContaner.SetProfitCallback(boost::bind(&ITradeSignalFliter::UpdateProfit,m_pTradeSignalFliter,_1,_2));
}

void version1Container::InitFliter()
{
	m_pTradeSignalFliter = new TradeSignalFliterDemo();
}
void version1Container::InitSignalProducer()
{
	boost::property_tree::ptree lConfig;
	read_xml("TradeSignalProduceRule.xml",lConfig);

	for( std::pair<std::string,boost::property_tree::ptree>  lSignalConfig : lConfig.get_child("TradeSignalProduceRule"))
	{
		std::string lTradeSignalProducerType  = lSignalConfig.second.get<std::string>("SignalType"); // useless because all info store in
		std::string lSignalConfigFile =  lSignalConfig.second.get<std::string>("ConfigFile");
		if(lTradeSignalProducerType == "TradeSignalProducerDemo1")
		{
			m_TradeSignalProducerVec.push_back(new TradeSignalProducerDemo1(lSignalConfigFile,m_pIndexContaner));
		}

	}


	
}
void version1Container::Start()
{
	m_pIndexContaner->Start();

}
void version1Container::Stop()
{
	m_pIndexContaner->Stop();
	for(auto lHandle :m_LibHandleVec)
	{
		FreeLibrary(lHandle);
	}
}

}