#include "TradeAccountDemo1.h"
#include "ITradeSignalExecutor.h"
#include "IDriver_TD.h"
#include "CloseExecutor_3Level.h"
#include <boost\bind.hpp>
namespace AT
{

	TradeAccountDemo1::TradeAccountDemo1( const std::string& aConfigFile, IDriver_TD* apTD,ITradeAccountObserver* apTradeAccountOB )
		:m_pTD(apTD)
		,m_pTradeAccountOB(apTradeAccountOB)
		,m_IsCompleteClose(true)
		,m_IsCompleteOpen(true)
		,m_totalProfit(0)
	{
	}

	TradeAccountDemo1::~TradeAccountDemo1(void)
	{
	}

	void TradeAccountDemo1::HandleTradeSignal( const TradeSignal& aTradeSignal )
	{
		m_LastTradeSignal = aTradeSignal;
		m_openExecutorID = std::string(aTradeSignal.m_ID,g_TradeSignalIDLength);
		if(m_OpenExecutorMap.find(m_openExecutorID) != m_OpenExecutorMap.end())
		{
		   boost::shared_ptr<TradeCommand> lTradeCommand = m_OpenExecutorMap[m_openExecutorID]->AddTarget(m_TargetVol,aTradeSignal.m_BuyOrSell,aTradeSignal.m_TriggerMarketData);
		   m_IsCompleteOpen = false;
		   DoTradeCommand(lTradeCommand);
		}
	}

	void TradeAccountDemo1::OnMarketDepth( const MarketData& aMarketDepth )
	{
		m_LastMarket = aMarketDepth;
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

		CloseSetting_3Level lCloseConfig;
		m_CloseExecutor.reset(new CloseExecutor_3Level(lCloseConfig));
		m_CloseExecutor->SetFinishedCallbakc(boost::bind(&TradeAccountDemo1::HandleCloseExecutorResult,this,_1,_2,_3,_4));
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
				strcpy_s(lpInput->m_operation.AccoutID ,cAccoutIDLength,m_AccountID.c_str());
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

	void TradeAccountDemo1::HandleOpenExecutorResult( int32_t aPrice, int32_t aVol,bool IsBuy,bool isFinishe )
	{
		m_IsCompleteOpen = isFinishe;
		
		if(!isFinishe)
		{
			boost::shared_ptr<TradeCommand> lTradeCommand =m_CloseExecutor->AddTarget(aVol,!m_LastTradeSignal.m_BuyOrSell,m_LastMarket);
			m_IsCompleteClose = false;
			DoTradeCommand(lTradeCommand);

		}

		m_totalProfit += aVol * aPrice * (IsBuy? -1 : 1);
		if(m_IsCompleteClose && m_IsCompleteOpen)
		{
			m_ProfitNotifyer(m_totalProfit/m_TargetVol,m_LastMarket.m_UpdateTime,this);
		}

	}

	void TradeAccountDemo1::HandleCloseExecutorResult( int32_t aPrice, int32_t aVol,bool IsBuy,bool isFinishe )
	{
		m_IsCompleteClose = isFinishe;
		m_totalProfit += aVol * aPrice * (IsBuy? -1 : 1);
		if(m_IsCompleteClose && m_IsCompleteOpen)
		{
			m_ProfitNotifyer(m_totalProfit/m_TargetVol,m_LastMarket.m_UpdateTime,this);
		}
	}

}