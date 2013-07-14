#include "LimitToCancelExecutor.h"
#include "IDriver_TD.h"
#include "ATLogger.h"
#include "LimitExecutor.h"
#include "ExecutorFactory.h"


#include <boost\property_tree\ptree.hpp>
#include <boost\property_tree\xml_parser.hpp>
using namespace std;
namespace AT
{

LimitToCancelExecutor::LimitToCancelExecutor( const std::string& aConfigFile )
	:ExecutorBase(aConfigFile)
	,m_Status(LimitToCancelStstus::BeforeBegin)
{
	InitFromConfigFile(aConfigFile);
}

void LimitToCancelExecutor::InitFromConfigFile( const std::string& aConfigFile )
{
	boost::property_tree::ptree lConfigPtree;
	read_xml(aConfigFile,lConfigPtree);
	m_CancelTimeVol = lConfigPtree.get<int>("ExecutorConfig.CancelTime");
	m_pLimitExecutor = ExecutorFactory::CreateExecutor(LimitExecutorType,aConfigFile);
	m_pLimitExecutor->SetTradeReportCallback(m_TradeReport);
	m_pLimitExecutor->SetCommandHandler(m_CommandHandle);

}


LimitToCancelExecutor::~LimitToCancelExecutor(void)
{
}

void LimitToCancelExecutor::DoAddExecution( ExecutorInput aExecutorInput )
{
	m_pLimitExecutor->AddExecution(aExecutorInput);
	m_EndTime = aExecutorInput.TriggerMarketData.m_UpdateTime + boost::posix_time::seconds(m_CancelTimeVol);
	m_ExecutionStatusBase = m_pLimitExecutor->GetExecutionStatus();
	ATLOG(L_INFO,"LimitToCancelExecutor Start New Task");
}
void LimitToCancelExecutor::DoOnMarketDepth( const AT::MarketData& aMarketDepth )
{

	switch (m_Status)
	{
	case AT::LimitToCancelExecutor::LimitToCancelStstus::Limit_OrderTime:
		if( aMarketDepth.m_UpdateTime >m_EndTime)
		{
			m_pLimitExecutor->Abrot();
			m_Status = LimitToCancelStstus::PendingCancel;
		}

		break;
	case AT::LimitToCancelExecutor::LimitToCancelStstus::BeforeBegin:
	case AT::LimitToCancelExecutor::LimitToCancelStstus::PendingCancel:
	case AT::LimitToCancelExecutor::LimitToCancelStstus::Finish:
		break;
	default:
		break;
	}
	m_pLimitExecutor->OnMarketDepth(aMarketDepth);
	
}

void LimitToCancelExecutor::DoOnRtnTrade( const AT::TradeUpdate& aTrade )
{
	 m_pLimitExecutor->OnRtnTrade(aTrade);
}

void LimitToCancelExecutor::DoOnRtnOrder( const AT::OrderUpdate& apOrder )
{
	 m_pLimitExecutor->OnRtnOrder(apOrder);
	 if(m_pLimitExecutor->GetExecutionStatus().IsFinised)
	 {
		 m_Status = LimitToCancelStstus::Finish;
	 }
}

AT::ExecutionStatus LimitToCancelExecutor::GetExecutionStatus()
{
	return m_pLimitExecutor->GetExecutionStatus();
}

void LimitToCancelExecutor::DoAbrot()
{
	if (m_Status == LimitToCancelStstus::Limit_OrderTime)
	{
		 m_pLimitExecutor->Abrot();
		 m_Status = LimitToCancelStstus::PendingCancel;
	}
	
}



}