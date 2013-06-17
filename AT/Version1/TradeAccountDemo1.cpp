#include "TradeAccountDemo1.h"
#include "ITradeSignalExecutor.h"
#include "IDriver_TD.h"
namespace AT
{

	TradeAccountDemo1::TradeAccountDemo1( const std::string& aConfigFile, IDriver_TD* apTD,ITradeAccountObserver* apTradeAccountOB )
		:m_pTD(apTD)
		,m_pTradeAccountOB(apTradeAccountOB)
	{
		
	}

	TradeAccountDemo1::~TradeAccountDemo1(void)
	{
	}

	void TradeAccountDemo1::HandleTradeSignal( const TradeSignal& aTradeSignal )
	{
		m_openExecutorID = std::string(aTradeSignal.m_ID,g_TradeSignalIDLength);
		if(m_OpenExecutorMap.find(m_openExecutorID) != m_OpenExecutorMap.end())
		{
		   boost::shared_ptr<TradeCommand> lTradeCommand = m_OpenExecutorMap[m_openExecutorID]->AddTarget(m_TargetVol,aTradeSignal.m_TriggerMarketData);
		   DoTradeCommand(lTradeCommand);
		}
	}

	void TradeAccountDemo1::OnMarketDepth( const MarketData& aMarketDepth )
	{
		  boost::shared_ptr<TradeCommand> lTradeCommand = m_OpenExecutorMap[m_openExecutorID]->OnMarketDepth(aMarketDepth);
		  DoTradeCommand(lTradeCommand);
		  lTradeCommand = m_CloseExecutor->OnMarketDepth(aMarketDepth);
		  DoTradeCommand(lTradeCommand);
	}

	void TradeAccountDemo1::OnRtnOrder( const OrderUpdate& apOrder )
	{
		boost::shared_ptr<TradeCommand> lTradeCommand = m_OpenExecutorMap[m_openExecutorID]->OnRtnOrder(apOrder);
		DoTradeCommand(lTradeCommand);
		lTradeCommand = m_CloseExecutor->OnRtnOrder(apOrder);
		DoTradeCommand(lTradeCommand);
	}

	void TradeAccountDemo1::OnRtnTrade( const TradeUpdate& apTrade )
	{
		boost::shared_ptr<TradeCommand> lTradeCommand = m_OpenExecutorMap[m_openExecutorID]->OnRtnTrade(apTrade);
		DoTradeCommand(lTradeCommand);
		lTradeCommand = m_CloseExecutor->OnRtnTrade(apTrade);
		DoTradeCommand(lTradeCommand);
	}

	void TradeAccountDemo1::InitFromConfigFile( const std::string& aConfigFile )
	{
		//todo load AccountID
		m_AccountID;

		//todo Load Signal OpenExecutor Map
		m_OpenExecutorMap;
		//todo Load Close Executor
		m_CloseExecutor;
	}	

	void TradeAccountDemo1::DoTradeCommand( boost::shared_ptr<TradeCommand> apTradeCommand )
	{
		switch (apTradeCommand->m_CommandType)
		{
		case TradeCommandType::Invalid:
				return;
		case TradeCommandType::Input:
			{
				InputCommand* lpInput = static_cast<InputCommand*> (apTradeCommand.get());
				strcpy(lpInput->m_operation.AccoutID ,m_AccountID.c_str());
				m_pTD->CreateOrder(lpInput->m_operation);
			}
			break;
		case TradeCommandType::Modify:
			{
				ModifyCommand* lpModify = static_cast<ModifyCommand*> (apTradeCommand.get());
				m_pTD->ModifyOrder(lpModify->m_operation);
			}
			break;			
		case TradeCommandType::Cancel:
			{
				CancelCommand* lpDelOrder = static_cast<CancelCommand*> (apTradeCommand.get());
				m_pTD->DeleteOrder(lpDelOrder->m_operation);
			}
			break;	
		default:
			break;
		}
	}

}