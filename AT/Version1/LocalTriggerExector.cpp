#include "LocalTriggerExector.h"
#include "IDriver_TD.h"
#include "ATLogger.h"

#include "ExecutorFactory.h"
namespace AT
{


LocalTriggerExector::LocalTriggerExector( const boost::property_tree::ptree& aConfigPtee )
	:ExecutorBase(aConfigPtee)
{
	m_StopLossOffset = aConfigPtee.get<int>("ExecutorConfig.StopOffset");
	m_TriggerExecutor = ExecutorFactory::CreateExecutor(aConfigPtee.get<std::string>("ExecutorConfig.TriggerType"),aConfigPtee);
	m_TriggerExecutor->SetCommandHandler(m_CommandHandle);
	m_TriggerExecutor->SetTradeReportCallback(m_TradeReport);
	m_Status = TriggerStatus::NotTrigger;
}


LocalTriggerExector::~LocalTriggerExector(void)
{
}

void LocalTriggerExector::DoAddExecution( ExecutorInput aExecutorInput )
{

	m_TriggerPrice = aExecutorInput.TriggerMarketData.m_LastPrice ;
	if(aExecutorInput.BuySellCode == BuySellType::BuyOrder)
	{
		m_TriggerPrice += m_StopLossOffset;
	}
	else
	{
		m_TriggerPrice -= m_StopLossOffset;
	}
	m_ExecutionStatusBase.AddTastVol = aExecutorInput.vol;
	m_ExecutionStatusBase.SuspendVol_Local = aExecutorInput.vol;
	m_ExecutionStatusBase.IsFinised = false;
	m_Input = aExecutorInput;

}


void LocalTriggerExector::DoOnMarketDepth( const AT::MarketData& aMarketDepth )
{

	if(m_Status == TriggerStatus::NotTrigger)
	{
		if(m_Input.BuySellCode == BuySellType::BuyOrder)
		{
			if(aMarketDepth.m_LastPrice  >= m_TriggerPrice)
			{
				m_Status = TriggerStatus::Triggerd;
				m_Input.TriggerMarketData =aMarketDepth;
				m_TriggerExecutor->AddExecution(m_Input);
			}
		}
		else
		{
			if(aMarketDepth.m_LastPrice <= m_TriggerPrice)
			{
				m_Status = TriggerStatus::Triggerd;
				m_Input.TriggerMarketData =aMarketDepth;
				m_TriggerExecutor->AddExecution(m_Input);
			}
		}
	}
	else
	{
		m_TriggerExecutor->OnMarketDepth(aMarketDepth);
	}

	return ;
}

void LocalTriggerExector::DoOnRtnTrade( const AT::TradeUpdate& aTrade )
{

	m_TriggerExecutor->OnRtnTrade(aTrade);
	return ;
}

void LocalTriggerExector::DoOnRtnOrder( const AT::OrderUpdate& aOrder )
{
	
	m_TriggerExecutor->OnRtnOrder(aOrder);
	m_ExecutionStatusBase = m_TriggerExecutor->GetExecutionStatus();
}
void LocalTriggerExector::DoAbrot()
{
	if (m_Status == TriggerStatus::Triggerd)
	{
		m_TriggerExecutor->Abrot();
	}
}

}