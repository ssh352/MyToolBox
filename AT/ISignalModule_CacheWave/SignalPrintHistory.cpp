#include "SignalPrintHistory.h"
#include "IMarketCache.h"
#include <boost\property_tree\ptree.hpp>
#include <boost\property_tree\xml_parser.hpp>
#include <MarketMapWarpper.h>
SignalPrintHistory::SignalPrintHistory(const char* aConfigFile,const AT::IMarketCache* apMarketCache)
	: m_pConstMarketCache(apMarketCache)
	, m_Configfile(aConfigFile)
{
}


SignalPrintHistory::~SignalPrintHistory(void)
{
}

void SignalPrintHistory::Start()
{
	boost::property_tree::ptree lConfig;
	read_xml(m_Configfile,lConfig);
	for(auto lItem: lConfig.get_child("PrintList"))
	{
		m_PrintList.insert(lItem.second.data());
	}

	for(std::string aInstrument:m_PrintList)
	{
		AT::const_MapWarpper_ptr lpMarketMap =	m_pConstMarketCache->GetMarketMapByName(aInstrument.c_str());
		for(const AT::MarketData& lMarket: (*lpMarketMap ))
		{
			std::cout<<lMarket.ToString()<<"\n";
		}
	}
	
}

int SignalPrintHistory::OnMarketDepth( const AT::MarketData& aMarketDepth )
{
	return 0;
}

void SignalPrintHistory::Stop()
{

}
