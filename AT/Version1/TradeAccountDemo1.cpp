#include "TradeAccountDemo1.h"
#include "ITradeSignalExecutor.h"
#include "IDriver_TD.h"
#include "CloseExecutor_3Level.h"
#include "OpenLimitExecutor.h"
#include "OpenMarketexecutor.h"
#include "OpenFollowExecutor.h"
#include <boost\bind.hpp>
#include <boost\property_tree\ptree.hpp>
#include <boost\property_tree\xml_parser.hpp>
#include "../AT_Driver/ATLogger.h"
namespace AT
{

	TradeAccountDemo1::TradeAccountDemo1( const std::string& aConfigFile, IDriver_TD* apTD )
		:m_pTD(apTD)
		,m_IsCompleteClose(true)
		,m_IsCompleteOpen(true)
		,m_totalProfit(0)
	{
		InitFromConfigFile(aConfigFile);
	}

	TradeAccountDemo1::~TradeAccountDemo1(void)
	{
	}

	void TradeAccountDemo1::HandleTradeSignal( const TradeSignal& aTradeSignal )
	{
		m_LastTradeSignal = aTradeSignal;
		m_openExecutorID = aTradeSignal.m_ID;
		if(m_OpenExecutorMap.find(m_openExecutorID) != m_OpenExecutorMap.end())
		{
			//单边持仓限额大于600
			if(m_OpenExecutorMap[m_openExecutorID]->GetSignalDirectionVol(aTradeSignal.m_BuyOrSell ? BuySellType::BuyOrder :BuySellType::SellOrder) >= 600)  
			{
				if(aTradeSignal.m_BuyOrSell)
				{
					ATLOG(LogLevel::L_ERROR,"买单持仓限额超过600手");
				}
				else
				{
					ATLOG(LogLevel::L_ERROR,"卖单持仓限额超过600手");
				}
				
				return;
			}
		   boost::shared_ptr<TradeCommand> lTradeCommand = m_OpenExecutorMap[m_openExecutorID]->AddTarget(m_TargetVol,aTradeSignal.m_BuyOrSell,aTradeSignal.m_TriggerMarketData);
		   m_IsCompleteOpen = false;
		   m_IsCompleteClose = false;
		   DoTradeCommand(lTradeCommand);
		}
	}

	void TradeAccountDemo1::OnMarketDepth( const MarketData& aMarketDepth )
	{
		m_LastMarket = aMarketDepth;
		boost::shared_ptr<TradeCommand> lTradeCommand;
		if(m_openExecutorID != "")
		{
			lTradeCommand = m_OpenExecutorMap[m_openExecutorID]->OnMarketDepth(aMarketDepth);
			DoTradeCommand(lTradeCommand);
		}
		  
		  lTradeCommand = m_CloseExecutor->OnMarketDepth(aMarketDepth);
		  DoTradeCommand(lTradeCommand);
	}

	void TradeAccountDemo1::OnRtnOrder( const OrderUpdate& apOrder )
	{
		boost::shared_ptr<TradeCommand> lTradeCommand;
		if(m_openExecutorID != "")
		{
			lTradeCommand = m_OpenExecutorMap[m_openExecutorID]->OnRtnOrder(apOrder);
			DoTradeCommand(lTradeCommand);
		}
		lTradeCommand = m_CloseExecutor->OnRtnOrder(apOrder);
		DoTradeCommand(lTradeCommand);
	}

	void TradeAccountDemo1::OnRtnTrade( const TradeUpdate& apTrade )
	{
		boost::shared_ptr<TradeCommand> lTradeCommand;
		if(m_openExecutorID != "")
		{
			lTradeCommand = m_OpenExecutorMap[m_openExecutorID]->OnRtnTrade(apTrade);
			DoTradeCommand(lTradeCommand);
		}
		lTradeCommand = m_CloseExecutor->OnRtnTrade(apTrade);
		DoTradeCommand(lTradeCommand);
	}

	void TradeAccountDemo1::InitFromConfigFile( const std::string& aConfigFile )
	{
		//todo load AccountID
		boost::property_tree::ptree lpt;
		read_xml(aConfigFile,lpt);

		m_AccountID = lpt.get<std::string>("AccountFile.AccountID");

		m_TargetVol = 1;

		//加载策略相关信息
		boost::property_tree::ptree ptStretegy;
		read_xml("StretegyConfig.xml",ptStretegy);
		std::string strSignalName;
		int iOrderExecutor;
		int iOrdersValidity;
		int iBufferPoint;
		int iFollowupPriority;
		for (std::pair<std::string,boost::property_tree::ptree> lSignal:ptStretegy.get_child("StretegyConfig.Signals"))
		{
			strSignalName = lSignal.second.get<std::string>("SignalName");
			iOrderExecutor = lSignal.second.get<int>("Open.OrderExecutor");
			iOrdersValidity = lSignal.second.get<int>("Open.OrdersValidity");
			iBufferPoint = lSignal.second.get<int>("Open.BufferPoint");
			iFollowupPriority = lSignal.second.get<int>("Open.FollowUpPriority");
			if(iOrderExecutor == 1)//市价
			{
				m_OpenExecutorMap[strSignalName].reset(new OpenMarketExecutor());
			}
			else if(iOrderExecutor == 2)//挂单
			{
				m_OpenExecutorMap[strSignalName].reset(new OpenLimitExecutor(iOrdersValidity,iBufferPoint));
			}
			else   //追加单
			{
				FollowExecutorParma follow;
				follow.m_FollowRange = 3;
				follow.m_FollowTime = 5;
				m_OpenExecutorMap[strSignalName].reset(new OpenFollowExecutor(follow));
			}
			m_OpenExecutorMap[strSignalName]->SetFinishedCallback(boost::bind(&TradeAccountDemo1::HandleOpenExecutorResult,this,_1,_2,_3,_4));
			m_OpenExecutorMap[strSignalName]->InitSignalDirectionVol();
			m_OpenExecutorMap[strSignalName]->InitTotalTime();
		}

		//todo Load Signal OpenExecutor Map
// 		m_OpenExecutorMap["TradingSignal01"].reset(new OpenLimitExecutor(30,-2));
// 		m_OpenExecutorMap["TradingSignal01"]->SetFinishedCallback(boost::bind(&TradeAccountDemo1::HandleOpenExecutorResult,this,_1,_2,_3,_4));
// 
// 		m_OpenExecutorMap["TradingSignal02"].reset(new OpenMarketExecutor());
// 		m_OpenExecutorMap["TradingSignal02"]->SetFinishedCallback(boost::bind(&TradeAccountDemo1::HandleOpenExecutorResult,this,_1,_2,_3,_4));

		//todo Load Close Executor

		CloseSetting_3Level lCloseConfig;
		lCloseConfig.QuitLevel_0 = lpt.get<int>("AccountFile.Level0.QuitPrice");
		lCloseConfig.QuitLevel_1 = lpt.get<int>("AccountFile.Level1.QuitPrice");
		lCloseConfig.QuitLevel_2 = lpt.get<int>("AccountFile.Level2.QuitPrice");
		lCloseConfig.QuitLevel_3 = lpt.get<int>("AccountFile.Level3.QuitPrice");
		lCloseConfig.EnterLevel_1 = lpt.get<int>("AccountFile.Level1.EnterPrice");
		lCloseConfig.EnterLevel_2 = lpt.get<int>("AccountFile.Level2.EnterPrice");
		lCloseConfig.EnterLevel_3 = lpt.get<int>("AccountFile.Level3.EnterPrice");
		std::string strStopTime = to_simple_string(boost::gregorian::day_clock::local_day())+" "+lpt.get<std::string>("AccountFile.StopTime");
		lCloseConfig.StopTime = boost::posix_time::time_from_string(strStopTime);
		std::string strStopClearTime = to_simple_string(boost::gregorian::day_clock::local_day())+" "+lpt.get<std::string>("AccountFile.StopClearTime");
		lCloseConfig.StopClearTime = boost::posix_time::time_from_string(strStopClearTime);
		m_CloseExecutor.reset(new CloseExecutor_3Level(lCloseConfig));
		m_CloseExecutor->SetFinishedCallback(boost::bind(&TradeAccountDemo1::HandleCloseExecutorResult,this,_1,_2,_3,_4));
		m_CloseExecutor->InitSignalDirectionVol();
		m_CloseExecutor->InitTotalTime();
	}	

	void TradeAccountDemo1::DoTradeCommand( boost::shared_ptr<TradeCommand> apTradeCommand )
	{
		switch (apTradeCommand->m_CommandType)
		{
		case TradeCommandType::Invalid:
				return;
		case TradeCommandType::Input:
			{
				if(m_openExecutorID != "")
				{
					//交易次数不能超过1000次
					if(m_OpenExecutorMap[m_openExecutorID]->GetTotalTime() > 1000)
					{
						ATLOG(LogLevel::L_ERROR,"交易次数已超过1000次");
						return;
					}
					m_OpenExecutorMap[m_openExecutorID]->SetTotalTime();
				}
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
				if(m_openExecutorID != "")
				{
					//撤单交易次数不能超过500次
					if(m_OpenExecutorMap[m_openExecutorID]->GetTotalCancleTime() > 500)
					{
						ATLOG(LogLevel::L_ERROR,"撤单交易次数已超过500次");
						return;
					}
					m_OpenExecutorMap[m_openExecutorID]->SetTotalCancleTime();
				}
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
			//boost::shared_ptr<TradeCommand> lTradeCommand =m_CloseExecutor->AddTarget(aVol,!m_LastTradeSignal.m_BuyOrSell,m_LastMarket);
			m_IsCompleteClose = false;
			//DoTradeCommand(lTradeCommand);

		}
		
		boost::shared_ptr<TradeCommand> lTradeCommand =m_CloseExecutor->AddTarget(aVol,!m_LastTradeSignal.m_BuyOrSell,m_LastMarket);
		DoTradeCommand(lTradeCommand);

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