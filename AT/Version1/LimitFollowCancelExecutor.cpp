#include "LimitFollowCancelExecutor.h"

#include "ExecutorFactory.h"
namespace AT
{



LimitFollowCancelExecutor::LimitFollowCancelExecutor( const boost::property_tree::ptree& aConfigPtee )
	:ExecutorBase(aConfigPtee)
{
	m_Status = ExecutorStatus::IDLE;
	m_pLimitExecutor = ExecutorFactory::CreateExecutor(LimitExecutorType,aConfigPtee);
	m_pFellowExecutor = ExecutorFactory::CreateExecutor(FollowExecutorType,aConfigPtee);

	m_pLimitExecutor->SetCommandHandler(m_CommandHandle);
	m_pFellowExecutor->SetCommandHandler(m_CommandHandle);

	boost::function<void(ExecutionResult aTrade)> FirstTradeReprotFun = boost::bind(&LimitFollowCancelExecutor::HandleFirstExecutorResult,this,_1);


	m_pLimitExecutor->SetTradeReportCallback(FirstTradeReprotFun);
	m_pFellowExecutor->SetTradeReportCallback(m_TradeReport);

	m_Limit2FollowTime = aConfigPtee.get<int>("ExecutorConfig.Limit2FollowTime");
	m_Follow2CancelTime = aConfigPtee.get<int>("ExecutorConfig.Follow2CancelTime");

}


LimitFollowCancelExecutor::~LimitFollowCancelExecutor(void)
{
}

void LimitFollowCancelExecutor::DoAddExecution( ExecutorInput aExecutorInput )
{
	m_ExecutorInput = aExecutorInput;
	m_Status = ExecutorStatus::LimitStatus;
	m_pLimitExecutor->AddExecution(aExecutorInput);
	m_StartTime = aExecutorInput.TriggerMarketData.m_UpdateTime;
}

void LimitFollowCancelExecutor::DoOnMarketDepth( const AT::MarketData& aMarketDepth )
{

	boost::posix_time::time_duration time_passed = aMarketDepth.m_UpdateTime - m_StartTime;


	switch (m_Status)
	{

	case AT::LimitFollowCancelExecutor::ExecutorStatus::LimitStatus:
		if(time_passed> boost::posix_time::seconds(m_Limit2FollowTime))
		{
			m_pLimitExecutor->Abrot();
			m_Status = ExecutorStatus::LimitChangeToFollowStatus;
		}
		break;

	case AT::LimitFollowCancelExecutor::ExecutorStatus::FollowSatus:
		if(time_passed> boost::posix_time::seconds(m_Follow2CancelTime))
		{
			m_pFellowExecutor->Abrot();
		}
		break;

	default:
		break;
	}

}

void LimitFollowCancelExecutor::DoOnRtnOrder( const AT::OrderUpdate& apOrder )
{
	switch (m_Status)
	{
	case ExecutorStatus::LimitAbortStatus:
	case ExecutorStatus::LimitStatus:
		m_pLimitExecutor->OnRtnOrder(apOrder);
		SetupTradeInfoBase(apOrder);
		if(m_pLimitExecutor->GetExecutionStatus().IsFinised)
		{
			m_ExecutionStatusBase.IsFinised = true;
			m_Status = ExecutorStatus::Finishe;
		}
		break;
	

	case ExecutorStatus::LimitChangeToFollowStatus:
		m_pLimitExecutor->OnRtnOrder(apOrder);
		SetupTradeInfoBase(apOrder);
		if(m_pLimitExecutor->GetExecutionStatus().IsFinised)
		{
			ExecutorInput lFollowInput = BuildFollowExecution() ;
			m_pFellowExecutor->AddExecution(lFollowInput);
		}
		break;
	case ExecutorStatus::FollowSatus:
		m_pFellowExecutor->OnRtnOrder(apOrder);
		if(m_pLimitExecutor->GetExecutionStatus().IsFinised)
		{
			m_ExecutionStatusBase.IsFinised = true;
			m_Status = ExecutorStatus::Finishe;
		}
		break;
	case ExecutorStatus::Finishe:
		break;
	default:
		break;
	}
}

AT::ExecutorInput LimitFollowCancelExecutor::BuildFollowExecution()
{
	int		leftVol = m_ExecutionStatusBase.AddTastVol - m_TradeQuantity;
	ExecutorInput lret = m_ExecutorInput;
	lret.vol = leftVol;
	return lret;

	//ExecutorInput lret;
	//return lret;
}

void LimitFollowCancelExecutor::HandleFirstExecutorResult( ExecutionResult aTrade )
{
	m_TradeQuantity += aTrade.vol;
	m_TradeReport(aTrade); 
}

}