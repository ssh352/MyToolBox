#include "LimitToCancelExecutor.h"
#include "IDriver_TD.h"
#include "ATLogger.h"
#include "LimitExecutor.h"

#include <boost\property_tree\ptree.hpp>
#include <boost\property_tree\xml_parser.hpp>
using namespace std;
namespace AT
{

LimitToCancelExecutor::LimitToCancelExecutor( const std::string& aConfigFile )
{
	InitFromConfigFile(aConfigFile);
	m_pLimitExecutor.reset(new LimitExecutor(aConfigFile));
	m_pLimitExecutor->SetTradeReportCallback(m_TradeReport);
	m_pLimitExecutor->SetCommandHandler(m_CommandHandle);
}

void LimitToCancelExecutor::InitFromConfigFile( const std::string& aConfigFile )
{
	boost::property_tree::ptree lConfigPtree;
	read_xml(aConfigFile,lConfigPtree);
	m_CancelTimeVol = lConfigPtree.get<int>("ExecutorConfig.CancelTime");
	m_ExecutorID = lConfigPtree.get<std::string>("ExecutorConfig.ExecutorID");

}


LimitToCancelExecutor::~LimitToCancelExecutor(void)
{
}

std::string LimitToCancelExecutor::GetExecutorID()
{
	return m_ExecutorID;
}

void LimitToCancelExecutor::AddExecution( ExecutorInput aExecutorInput )
{
	if(m_pLimitExecutor->GetExecutionStatus().IsFinised != false)
	{
		m_pLimitExecutor->AddExecution(aExecutorInput);
		m_EndTime = aExecutorInput.LastMarketData.m_UpdateTime + boost::posix_time::seconds(m_CancelTimeVol);
		ATLOG(L_INFO,"LimitToCancelExecutor Start New Task");
	}
	else
	{
		ATLOG(L_ERROR,"LimitToCancelExecutor DId not Finnish Last Task");
		return ;
	}

}
void LimitToCancelExecutor::OnMarketDepth( const AT::MarketData& aMarketDepth )
{
	if (m_pLimitExecutor->GetExecutionStatus().IsFinised == false && aMarketDepth.m_UpdateTime >m_EndTime)
	{
		 m_pLimitExecutor->Abrot();
	}
	else
	{
		 m_pLimitExecutor->OnMarketDepth(aMarketDepth);
	}
}

void LimitToCancelExecutor::OnRtnTrade( const AT::TradeUpdate& aTrade )
{
	 m_pLimitExecutor->OnRtnTrade(aTrade);
}

void LimitToCancelExecutor::OnRtnOrder( const AT::OrderUpdate& apOrder )
{
	 m_pLimitExecutor->OnRtnOrder(apOrder);
}

AT::ExecutionStatus LimitToCancelExecutor::GetExecutionStatus()
{
	return m_pLimitExecutor->GetExecutionStatus();
}

void LimitToCancelExecutor::Abrot()
{
	 m_pLimitExecutor->Abrot();
}



}