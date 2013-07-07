#include "ExecutorContianer.h"
#include "ATLogger.h"
#include "ExecutorFactory.h"
#include "boost/property_tree/ptree.hpp"
#include "boost/property_tree/xml_parser.hpp"


namespace AT
{

ExecutorContianer::ExecutorContianer(const std::string& aConfigFile )
{
	InitFromConfig(aConfigFile);
}


ExecutorContianer::~ExecutorContianer(void)
{
}

void ExecutorContianer::InitFromConfig( const std::string& aConfigFile )
{
	boost::property_tree::ptree lConfig;
	read_xml(aConfigFile,lConfig);
	for( std::pair<std::string,boost::property_tree::ptree>  lExecutorItem : lConfig.get_child("Executors"))
	{
		std::string lExecutorType = lExecutorItem.second.get<std::string>("ExecutorType");
		std::string lExecuorConfig =  lExecutorItem.second.get<std::string>("ExectorConfigFile");
		boost::shared_ptr<IExecutor> lExecutor = ExecutorFactory::CreateExecutor(lExecutorType,lExecuorConfig);
		m_ExecutorGruop.insert(make_pair(lExecutor->GetExecutorID(),lExecutor));
	}
}

boost::shared_ptr<IExecutor> ExecutorContianer::GetExecutorByID( const std::string& aExecutorID )
{
	if(m_ExecutorGruop.find(aExecutorID) != m_ExecutorGruop.end())
	{
		ATLOG(L_ERROR,"Cant Find Executor With ID");
		ATLOG(L_ERROR,aExecutorID);
		return boost::shared_ptr<IExecutor>();
	}
	return m_ExecutorGruop[aExecutorID];
}

std::vector<Command> ExecutorContianer::OnMarketDepth( const AT::MarketData& aMarketDepth )
{
	std::vector<Command> lret;
	for  (auto lExecutor : m_ExecutorGruop)
	{
		Command lTradeCommand = lExecutor.second->OnMarketDepth(aMarketDepth);
		lret.push_back(lTradeCommand);
	}
	return lret;
}

std::vector<Command> ExecutorContianer::OnRtnOrder( const AT::OrderUpdate& apOrder )
{
	std::vector<Command> lret;
	for  (auto lExecutor : m_ExecutorGruop)
	{
		Command lTradeCommand = lExecutor.second->OnRtnOrder(apOrder);
		lret.push_back(lTradeCommand);
	}

	return lret;
}

std::vector<Command> ExecutorContianer::OnRtnTrade( const AT::TradeUpdate& apTrade )
{
	std::vector<Command> lret;
	for  (auto lExecutor : m_ExecutorGruop)
	{
		Command lTradeCommand = lExecutor.second->OnRtnTrade(apTrade);
		lret.push_back(lTradeCommand);

	}
	return lret;
}

}