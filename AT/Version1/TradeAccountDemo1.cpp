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
#include <boost\filesystem.hpp>
namespace AT
{

	TradeAccountDemo1::TradeAccountDemo1( const std::string& aConfigFile, IDriver_TD* apTD )
		:m_pTD(apTD)
		,m_IsCompleteClose(true)
		,m_IsCompleteOpen(true)
		,m_totalProfit(0)
	{
		//InitFromConfigFile(aConfigFile);
		InitExchangeRule();
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
			int iSinglePositionVol = aTradeSignal.m_BuyOrSell ? m_BuyDirectionVol:m_SellDirectionVol;
			if( iSinglePositionVol > m_SinglePositionVol )  
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
			//最大累计开仓手数
			if (m_TotalOpenVol > m_TotalMaxOpenVol)
			{
				ATLOG(LogLevel::L_ERROR,"超过最大累计开仓手数");
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
		//撤单成功
		if(apOrder.m_OrderStatus == OrderStatusType::StoppedOrder)
		{
			SetTotalCancleTime();
		}
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
		//单边持仓限额
		SetSignalDirectionVol(apTrade.m_BuySellType,apTrade.m_TradeVol,apTrade.m_OpenCloseType == OpenCloseType::OpenOrder ? true:false);
		if(apTrade.m_OpenCloseType == OpenCloseType::OpenOrder)
		{
			SetTotalOpenVol(apTrade.m_TradeVol);
		}
		
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

		m_TargetVol = lpt.get<int>("AccountFile.TargetVol");

		m_ExchangePath = lpt.get<std::string>("AccountFile.ExchangeRule");

		//创建存储交易所相关数量的文件夹
		boost::filesystem::path lDir(m_ExchangePath);

		std::string lDataString = boost::gregorian::to_iso_string(AT::AT_Local_Time().date());

		if (!boost::filesystem::exists(lDir))
		{
			create_directory(lDir);
		}
		lDir /= lDataString;
		if(!boost::filesystem::exists(lDir))
		{
			create_directory(lDir);
		}
		m_TradeVolDB.reset(new SingleDBHandler(lDir.string().c_str()));

		//加载策略相关信息
		boost::property_tree::ptree ptStretegy;
		read_xml("StretegyConfig.xml",ptStretegy);
		boost::property_tree::ptree ptOpen;
		read_xml("OpenExecutorConfig.xml",ptOpen);
		boost::property_tree::ptree ptClose;
		read_xml("CloseExecutorConfig.xml",ptClose);
		std::string strSignalName;
		int iOrderExecutor;
		int iOrdersValidity;
		int iBufferPoint;
		int iFollowupPriority;
		for (std::pair<std::string,boost::property_tree::ptree> lSignal:ptStretegy.get_child("StretegyConfig.Signals"))
		{
			strSignalName = lSignal.second.get<std::string>("SignalID");
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
			else   //追价单
			{
				FollowExecutorParma follow;
				follow.m_FollowRange = 3;
				follow.m_FollowTime = 5;
				m_OpenExecutorMap[strSignalName].reset(new OpenFollowExecutor(follow));
			}
			m_OpenExecutorMap[strSignalName]->SetFinishedCallback(boost::bind(&TradeAccountDemo1::HandleOpenExecutorResult,this,_1,_2,_3,_4));
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
				if(m_openExecutorID != "")
				{
					//限价单限制
					if(lpInput->m_operation.m_OrderType == OrderType::LimitOrder)
					{
						if(lpInput->m_operation.m_Vol > m_LimitMaxVol)
						{
							ATLOG(LogLevel::L_ERROR,"限价单超过最大手数");
							return;
						}
					}
					else if(lpInput->m_operation.m_OrderType == OrderType::MarketOrder)//市价单
					{
						if(lpInput->m_operation.m_Vol > m_MarketMaxVol)
						{
							ATLOG(LogLevel::L_ERROR,"市价单超过最大手数");
							return;
						}
					}
				}
				
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
					if(m_TotalCancleTime > m_MaxCancleTime)
					{
						ATLOG(LogLevel::L_ERROR,"撤单交易次数已超过500次");
						return;
					}
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
		if(aPrice == 0 && aVol == 0)
		{
			m_IsCompleteClose = true;
		}
		//平仓
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
	void TradeAccountDemo1::InitExchangeRule()
	{
		boost::property_tree::ptree ptExchange;
		read_xml("ExchangeRuleConfig.xml",ptExchange);
		m_LimitMaxVol = ptExchange.get<int>("ExchangeRuluConfig.LimitMaxVol");
		m_MarketMaxVol = ptExchange.get<int>("ExchangeRuluConfig.MarketMaxVol");
		m_MaxCancleTime = ptExchange.get<int>("ExchangeRuluConfig.MaxCancleTimeVol");
		m_TotalMaxOpenVol = ptExchange.get<int>("ExchangeRuluConfig.MaxTotalOpenVol");
		m_SinglePositionVol = ptExchange.get<int>("ExchangeRuluConfig.SinglePositionMaxVol");
		m_AutoTradeMaxTime = ptExchange.get<int>("ExchangeRuluConfig.AutoTradeMaxTime");

		RestoreTradeVol();
	}
	void TradeAccountDemo1::SetSignalDirectionVol(BuySellType type,int Vol,bool bAdd)
	{
		if(type == BuySellType::BuyOrder)
		{
			if(bAdd)
			{
				m_BuyDirectionVol += Vol;
			}
			else
			{
				m_BuyDirectionVol -= Vol;
			}
		}
		else
		{
			if (bAdd)
			{
				m_SellDirectionVol += Vol;
			}
			else
			{
				m_SellDirectionVol -= Vol;
			}
		}
		StoreTradeVol();
	}
	void TradeAccountDemo1::StoreTradeVol()
	{
		std::shared_ptr<AT::TradeVolData> pTradeVolData(new TradeVolData);
		pTradeVolData->m_BuyDirectionVol = m_BuyDirectionVol;
		pTradeVolData->m_SellDirectionVol = m_SellDirectionVol;
		pTradeVolData->m_TotalCancleTime = m_TotalCancleTime;
		pTradeVolData->m_TotalOpenVol = m_TotalOpenVol;
		pTradeVolData->m_AutoTradeTime = m_AutoTradeTime;
		m_TradeVolDB->StoreTradeVolData(pTradeVolData);
	}
	void TradeAccountDemo1::RestoreTradeVol()
	{
		std::shared_ptr<AT::TradeVolData> pTradeVolData(new TradeVolData);
		m_TradeVolDB->RestoreTradeVolData(pTradeVolData);
		m_BuyDirectionVol = pTradeVolData->m_BuyDirectionVol;
		m_SellDirectionVol = pTradeVolData->m_SellDirectionVol;
		m_TotalCancleTime = pTradeVolData->m_TotalCancleTime;
		m_TotalOpenVol = pTradeVolData->m_TotalOpenVol;
		m_AutoTradeTime = pTradeVolData->m_AutoTradeTime;
	}
}