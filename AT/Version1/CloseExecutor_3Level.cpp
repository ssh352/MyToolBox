#include "CloseExecutor_3Level.h"
#include "IDriver_TD.h"
#include "ATLogger.h"
#include "ExecutorFactory.h"

using namespace std;
namespace AT
{


CloseExecutor_3Level::CloseExecutor_3Level( const boost::property_tree::ptree& aConfigPtee )
	:ExecutorBase(aConfigPtee)
{
	InitCheckLevelSetting(aConfigPtee);
	InitChildExecutor(aConfigPtee);
	m_CurrentLevel = (CheckStatusLevel::Level0);
	m_StatusEnumCode = Close3LevelStatus::BeforeStart;
	m_MaxPriceDiff=(0);
	m_StartPrice=(0);
	m_TradeQuantity=(0);
}

CloseExecutor_3Level::~CloseExecutor_3Level(void)
{
}

void CloseExecutor_3Level::DoAddExecution( ExecutorInput aExecutorInput )
{

	m_ExecutionStatusBase.AddTastVol = aExecutorInput.vol;
	m_ExecutionStatusBase.IsFinised = false;
	m_pFirstExecutor->AddExecution(aExecutorInput);
	m_StatusEnumCode = Close3LevelStatus::StopLoss_CheckTrigger;
	m_BuySellCode = aExecutorInput.BuySellCode;
	m_ExecutorInput = aExecutorInput;
	m_MaxPriceDiff = 0;
	m_StartPrice = aExecutorInput.TriggerMarketData.m_LastPrice;
	
}

void CloseExecutor_3Level::HandleFirstExecutorResult( ExecutionResult aTrade )
{
	m_TradeQuantity += aTrade.vol;
	m_TradeReport(aTrade);
}

void CloseExecutor_3Level::HandleQuitExecutorResult( ExecutionResult aTrade )
{
	m_TradeReport(aTrade);
}

bool CloseExecutor_3Level::CheckTrigger( const AT::MarketData& aMarketDepth )
{
	int lPriceDiffStart = aMarketDepth.m_LastPrice - m_StartPrice;
	

	int AbsPriceDiff = lPriceDiffStart;
	if (m_BuySellCode == BuySellType::BuyOrder)
	{
		AbsPriceDiff = -lPriceDiffStart;
	}

	 UpdatePriceLevel(AbsPriceDiff);

	 m_MaxPriceDiff = max(m_MaxPriceDiff,AbsPriceDiff);
	 int FallBackFromHigh = m_MaxPriceDiff - AbsPriceDiff;
	 
	 switch (m_CurrentLevel)
	 {
	 case CheckStatusLevel::Level1:
		 if (FallBackFromHigh >= m_Setting.QuitLevel_1)
		 {
			return true;
		 }
		 break;
	 case  CheckStatusLevel::Level2:
		 if (FallBackFromHigh >= m_Setting.QuitLevel_2)
		 {
			return true;
		 }
		 break;
	 case CheckStatusLevel::Level3:
		 if(FallBackFromHigh >= m_Setting.QuitLevel_3)
		 {
			 return true;
		 }
		 break;
	 default:
		 break;
	 }

	 return false;

}

void CloseExecutor_3Level::UpdatePriceLevel( int AbsPriceDiff )
{
	switch (m_CurrentLevel)
	{
	case CheckStatusLevel::Level0:
		if (AbsPriceDiff >= m_Setting.EnterLevel_1)
		{
			m_CurrentLevel  = CheckStatusLevel::Level1;
		}
		break;
	case  CheckStatusLevel::Level1:
		if (AbsPriceDiff >= m_Setting.EnterLevel_2)
		{
			m_CurrentLevel  = CheckStatusLevel::Level2;
		}
		break;
	case CheckStatusLevel::Level2:
		if(AbsPriceDiff >= m_Setting.EnterLevel_3)
		{
			m_CurrentLevel  = CheckStatusLevel::Level3;
		}
		break;
	case CheckStatusLevel::Level3:
		break;
	default:
		break;
	}
}



AT::ExecutorInput CloseExecutor_3Level::BuildQuitExecution()
{

	int		leftVol = m_ExecutionStatusBase.AddTastVol - m_TradeQuantity;
	ExecutorInput lret = m_ExecutorInput;
	lret.vol = leftVol;
	return lret;
}

void CloseExecutor_3Level::InitChildExecutor( const boost::property_tree::ptree &lConfigPtree )
{
	std::string StopLossExecutorType = lConfigPtree.get<std::string>("ExecutorConfig.StopLossExecutorType");
	std::string StopLossExecutorConfig = lConfigPtree.get<std::string>("ExecutorConfig.StopLossExecutorConfig");

	std::string QuitExecutorType = lConfigPtree.get<std::string>("ExecutorConfig.QuitExecutorType");
	std::string QuitExecutorConfig = lConfigPtree.get<std::string>("ExecutorConfig.QuitExecutorConfig");

	m_pFirstExecutor = ExecutorFactory::CreateExecutor(StopLossExecutorType,StopLossExecutorConfig);
	m_pQuitExecutor = ExecutorFactory::CreateExecutor(QuitExecutorType,QuitExecutorConfig);

	m_pFirstExecutor->SetCommandHandler(m_CommandHandle);
	m_pQuitExecutor->SetCommandHandler(m_CommandHandle);

	boost::function<void(ExecutionResult aTrade)> FirstTradeReprotFun = boost::bind(&CloseExecutor_3Level::HandleFirstExecutorResult,this,_1);
	boost::function<void(ExecutionResult aTrade)> QuitTradeReprotFun = boost::bind(&CloseExecutor_3Level::HandleQuitExecutorResult,this,_1);

	m_pFirstExecutor->SetTradeReportCallback(FirstTradeReprotFun);
	m_pQuitExecutor->SetTradeReportCallback(QuitTradeReprotFun);
}

void CloseExecutor_3Level::InitCheckLevelSetting(const boost::property_tree::ptree &lConfigPtree )
{
	m_Setting.EnterLevel_1 = lConfigPtree.get<int>("ExecutorConfig.LevelEnter_1");
	m_Setting.QuitLevel_1 = lConfigPtree.get<int>("ExecutorConfig.LevelBack_1");
	m_Setting.EnterLevel_2 = lConfigPtree.get<int>("ExecutorConfig.LevelEnter_2");
	m_Setting.QuitLevel_2 = lConfigPtree.get<int>("ExecutorConfig.LevelBack_2");
	m_Setting.EnterLevel_3 = lConfigPtree.get<int>("ExecutorConfig.LevelEnter_3");
	m_Setting.QuitLevel_3 = lConfigPtree.get<int>("ExecutorConfig.LevelBack_3");
}

void CloseExecutor_3Level::DoAbrot()
{
	switch (m_StatusEnumCode)
	{
	case AT::CloseExecutor_3Level::Close3LevelStatus::BeforeStart:
		m_StatusEnumCode = Close3LevelStatus::Finished;
		m_ExecutionStatusBase.IsFinised = true;
		break;
	case AT::CloseExecutor_3Level::Close3LevelStatus::StopLoss_CheckTrigger:
		m_pFirstExecutor->Abrot();
		m_StatusEnumCode = Close3LevelStatus::AbrottingTime;
		break;
	case AT::CloseExecutor_3Level::Close3LevelStatus::PendingStopLossCancel_Time:
		m_StatusEnumCode = Close3LevelStatus::AbrottingTime;
		break;
	case AT::CloseExecutor_3Level::Close3LevelStatus::QuitExecutor_Time:
		m_StatusEnumCode = Close3LevelStatus::AbrottingTime;
		m_pQuitExecutor->Abrot();
		break;
	case AT::CloseExecutor_3Level::Close3LevelStatus::AbrottingTime:
		ATLOG(L_WARN,"Already in Aborting");
		break;
	case AT::CloseExecutor_3Level::Close3LevelStatus::Finished:
		ATLOG(L_WARN,"Already is Finished");
		break;
	default:
		break;
	}
}

void CloseExecutor_3Level::DoOnMarketDepth( const AT::MarketData& aMarketDepth )
{

	m_ExecutorInput.TriggerMarketData = aMarketDepth;

	switch (m_StatusEnumCode)
	{
	case AT::CloseExecutor_3Level::Close3LevelStatus::StopLoss_CheckTrigger:
		if(CheckTrigger(aMarketDepth))
		{
			m_pFirstExecutor->Abrot();
			m_StatusEnumCode = Close3LevelStatus::PendingStopLossCancel_Time;
		}
		else
		{
			m_pFirstExecutor->OnMarketDepth(aMarketDepth);
		}
		break;
	case AT::CloseExecutor_3Level::Close3LevelStatus::QuitExecutor_Time:
		m_pQuitExecutor->OnMarketDepth(aMarketDepth);
		break;
	case AT::CloseExecutor_3Level::Close3LevelStatus::BeforeStart:
	case AT::CloseExecutor_3Level::Close3LevelStatus::PendingStopLossCancel_Time:	
	case AT::CloseExecutor_3Level::Close3LevelStatus::AbrottingTime:
	case AT::CloseExecutor_3Level::Close3LevelStatus::Finished:
		break;
	default:
		break;
	}
}

void CloseExecutor_3Level::DoOnRtnOrder( const AT::OrderUpdate& apOrder )
{
	switch (m_StatusEnumCode)
	{
	case AT::CloseExecutor_3Level::Close3LevelStatus::BeforeStart:
		break;
	case AT::CloseExecutor_3Level::Close3LevelStatus::StopLoss_CheckTrigger:
		m_pFirstExecutor->OnRtnOrder(apOrder);
		if(m_pFirstExecutor->GetExecutionStatus().IsFinised)
		{
			ATLOG(L_INFO,"StopLoss Occur");
			m_StatusEnumCode = Close3LevelStatus::Finished;
			m_ExecutionStatusBase.IsFinised = true;
		}
		break;
	case AT::CloseExecutor_3Level::Close3LevelStatus::PendingStopLossCancel_Time:
		m_pFirstExecutor->OnRtnOrder(apOrder);
		if(m_pFirstExecutor->GetExecutionStatus().IsFinised)
		{
			ATLOG(L_INFO,"StopLoss Cancel");
			m_StatusEnumCode = Close3LevelStatus::QuitExecutor_Time;
			ExecutorInput lQuitInput = BuildQuitExecution();
			m_pQuitExecutor->AddExecution(lQuitInput);
		}
		break;
	case AT::CloseExecutor_3Level::Close3LevelStatus::QuitExecutor_Time:
		m_pQuitExecutor->OnRtnOrder(apOrder);
		if(m_pQuitExecutor->GetExecutionStatus().IsFinised)
		{
			m_StatusEnumCode = Close3LevelStatus::Finished;
			m_ExecutionStatusBase.IsFinised = true;
		}
		break;
	case AT::CloseExecutor_3Level::Close3LevelStatus::AbrottingTime:
		m_pFirstExecutor->OnRtnOrder(apOrder);
		m_pQuitExecutor->OnRtnOrder(apOrder);
		if (m_pFirstExecutor->GetExecutionStatus().IsFinised && m_pQuitExecutor->GetExecutionStatus().IsFinised)
		{
			m_StatusEnumCode = Close3LevelStatus::Finished;
			m_ExecutionStatusBase.IsFinised = true;
		}
		break;
	case AT::CloseExecutor_3Level::Close3LevelStatus::Finished:
		break;
	default:
		break;
	}
}

void CloseExecutor_3Level::DoOnRtnTrade( const AT::TradeUpdate& apTrade )
{
	m_pFirstExecutor->OnRtnTrade(apTrade);
	m_pQuitExecutor->OnRtnTrade(apTrade);
}


}