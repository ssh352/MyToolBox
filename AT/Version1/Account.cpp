#include "Account.h"
#include "IExecutor.h"
#include "IDriver_TD.h"
#include "CloseExecutor_3Level.h"
#include "OpenLimitExecutor.h"
#include "OpenMarketexecutor.h"
#include "OpenFollowExecutor.h"
#include <boost\bind.hpp>
#include <boost\property_tree\ptree.hpp>
#include <boost\property_tree\xml_parser.hpp>
#include "ATLogger.h"
#include <boost\filesystem.hpp>
#include "ExecutorContianer.h"
namespace AT
{

	Account::Account( const std::string& aConfigFile, IDriver_TD* apTD )
		:m_pTD(apTD)
	{
		InitFromConfigFile(aConfigFile);
	}

	void Account::InitFromConfigFile( const std::string& aConfigFile )
	{
		boost::property_tree::ptree lpt;
		read_xml(aConfigFile,lpt);

		m_AccountID = lpt.get<std::string>("AccountFile.AccountID");
		std::string lExchangRuleFile =  lpt.get<std::string>("AccountFile.ExchangeRule");
		InitExchangeRule(lExchangRuleFile);
		m_ExchangePath = lpt.get<std::string>("AccountFile.ExchangSavePath");


		std::string LExecutorContainerConfig =  lpt.get<std::string>("AccountFile.ExecutorConfig");
		InitExecutorContainer(LExecutorContainerConfig);

		m_TargetVol = lpt.get<int>("AccountFile.TargetVol");

		InitExchangeStroe();
	}	

	Account::~Account(void)
	{
	}

	//void Account::HandleTradeSignal( const Signal& aTradeSignal )
	//{
	//	m_LastTradeSignal = aTradeSignal;
	//	m_openExecutorID = aTradeSignal.m_ID;
	//	if(m_OpenExecutorMap.find(m_openExecutorID) != m_OpenExecutorMap.end())
	//	{
	//		//单边持仓限额大于600
	//		int iSinglePositionVol = aTradeSignal.m_BuyOrSell ? m_BuyDirectionVol:m_SellDirectionVol;
	//		if( iSinglePositionVol > m_SinglePositionVol )  
	//		{
	//			if(aTradeSignal.m_BuyOrSell)
	//			{
	//				ATLOG(LogLevel::L_ERROR,"买单持仓限额超过600手");
	//			}
	//			else
	//			{
	//				ATLOG(LogLevel::L_ERROR,"卖单持仓限额超过600手");
	//			}
	//			
	//			return;
	//		}
	//		//最大累计开仓手数
	//		if (m_TotalOpenVol > m_TotalMaxOpenVol)
	//		{
	//			ATLOG(LogLevel::L_ERROR,"超过最大累计开仓手数");
	//			return;
	//		}
	//	   boost::shared_ptr<TradeCommand> lTradeCommand = m_OpenExecutorMap[m_openExecutorID]->AddTarget(m_TargetVol,aTradeSignal.m_BuyOrSell,aTradeSignal.m_TriggerMarketData);
	//	   m_IsCompleteOpen = false;
	//	   m_IsCompleteClose = false;
	//	   DoTradeCommand(lTradeCommand);
	//	}
	//}

	//void Account::OnMarketDepth( const MarketData& aMarketDepth )
	//{
	//	m_LastMarket = aMarketDepth;
	//	boost::shared_ptr<TradeCommand> lTradeCommand;
	//	if(m_openExecutorID != "")
	//	{
	//		lTradeCommand = m_OpenExecutorMap[m_openExecutorID]->OnMarketDepth(aMarketDepth);
	//		DoTradeCommand(lTradeCommand);
	//	}
	//	  
	//	  lTradeCommand = m_CloseExecutor->OnMarketDepth(aMarketDepth);
	//	  DoTradeCommand(lTradeCommand);
	//}

	//void Account::OnRtnOrder( const OrderUpdate& apOrder )
	//{
	//	//撤单成功
	//	if(apOrder.m_OrderStatus == OrderStatusType::StoppedOrder)
	//	{
	//		SetTotalCancleTime();
	//	}
	//	boost::shared_ptr<TradeCommand> lTradeCommand;
	//	if(m_openExecutorID != "")
	//	{
	//		lTradeCommand = m_OpenExecutorMap[m_openExecutorID]->OnRtnOrder(apOrder);
	//		DoTradeCommand(lTradeCommand);
	//	}
	//	lTradeCommand = m_CloseExecutor->OnRtnOrder(apOrder);
	//	DoTradeCommand(lTradeCommand);
	//}

	//void Account::OnRtnTrade( const TradeUpdate& apTrade )
	//{
	//	//单边持仓限额
	//	SetSignalDirectionVol(apTrade.m_BuySellType,apTrade.m_TradeVol,apTrade.m_OpenCloseType == OpenCloseType::OpenOrder ? true:false);
	//	if(apTrade.m_OpenCloseType == OpenCloseType::OpenOrder)
	//	{
	//		SetTotalOpenVol(apTrade.m_TradeVol);
	//	}
	//	
	//	boost::shared_ptr<TradeCommand> lTradeCommand;
	//	if(m_openExecutorID != "")
	//	{
	//		lTradeCommand = m_OpenExecutorMap[m_openExecutorID]->OnRtnTrade(apTrade);
	//		DoTradeCommand(lTradeCommand);
	//	}
	//	lTradeCommand = m_CloseExecutor->OnRtnTrade(apTrade);
	//	DoTradeCommand(lTradeCommand);
	//}



	//void Account::DoTradeCommand( boost::shared_ptr<TradeCommand> apTradeCommand )
	//{
	//	switch (apTradeCommand->m_CommandType)
	//	{
	//	case TradeCommandType::Invalid:
	//			return;
	//	case TradeCommandType::Input:
	//		{
	//			InputCommand* lpInput = static_cast<InputCommand*> (apTradeCommand.get());
	//			strcpy_s(lpInput->m_operation.AccoutID ,cAccoutIDLength,m_AccountID.c_str());
	//			if(m_openExecutorID != "")
	//			{
	//				//限价单限制
	//				if(lpInput->m_operation.m_OrderType == OrderType::LimitOrder)
	//				{
	//					if(lpInput->m_operation.m_Vol > m_LimitMaxVol)
	//					{
	//						ATLOG(LogLevel::L_ERROR,"限价单超过最大手数");
	//						return;
	//					}
	//				}
	//				else if(lpInput->m_operation.m_OrderType == OrderType::MarketOrder)//市价单
	//				{
	//					if(lpInput->m_operation.m_Vol > m_MarketMaxVol)
	//					{
	//						ATLOG(LogLevel::L_ERROR,"市价单超过最大手数");
	//						return;
	//					}
	//				}
	//			}
	//			
	//			m_pTD->CreateOrder(lpInput->m_operation);
	//		}
	//		break;
	//	case TradeCommandType::Modify:
	//		{
	//			ModifyCommand* lpModify = static_cast<ModifyCommand*> (apTradeCommand.get());
	//			m_pTD->ModifyOrder(lpModify->m_operation);
	//		}
	//		break;			
	//	case TradeCommandType::Cancel:
	//		{
	//			if(m_openExecutorID != "")
	//			{
	//				//撤单交易次数不能超过500次
	//				if(m_TotalCancleTime > m_MaxCancleTime)
	//				{
	//					ATLOG(LogLevel::L_ERROR,"撤单交易次数已超过500次");
	//					return;
	//				}
	//			}
	//			CancelCommand* lpDelOrder = static_cast<CancelCommand*> (apTradeCommand.get());
	//			m_pTD->DeleteOrder(lpDelOrder->m_operation);
	//		}
	//		break;	
	//	default:
	//		break;
	//	}
	//}

	//void Account::HandleOpenExecutorResult( int32_t aPrice, int32_t aVol,bool IsBuy,bool isFinishe )
	//{
	//	m_IsCompleteOpen = isFinishe;
	//	
	//	if(!isFinishe)
	//	{
	//		//boost::shared_ptr<TradeCommand> lTradeCommand =m_CloseExecutor->AddTarget(aVol,!m_LastTradeSignal.m_BuyOrSell,m_LastMarket);
	//		m_IsCompleteClose = false;
	//		//DoTradeCommand(lTradeCommand);

	//	}
	//	if(aPrice == 0 && aVol == 0)
	//	{
	//		m_IsCompleteClose = true;
	//	}
	//	//平仓
	//	boost::shared_ptr<TradeCommand> lTradeCommand =m_CloseExecutor->AddTarget(aVol,!m_LastTradeSignal.m_BuyOrSell,m_LastMarket);
	//	DoTradeCommand(lTradeCommand);

	//	m_totalProfit += aVol * aPrice * (IsBuy? -1 : 1);
	//	if(m_IsCompleteClose && m_IsCompleteOpen)
	//	{
	//		//m_ProfitNotifyer(m_totalProfit/m_TargetVol,m_LastMarket.m_UpdateTime,this);
	//	}

	//}

	//void Account::HandleCloseExecutorResult( int32_t aPrice, int32_t aVol,bool IsBuy,bool isFinishe )
	//{
	//	m_IsCompleteClose = isFinishe;
	//	m_totalProfit += aVol * aPrice * (IsBuy? -1 : 1);
	//	if(m_IsCompleteClose && m_IsCompleteOpen)
	//	{
	//		//m_ProfitNotifyer(m_totalProfit/m_TargetVol,m_LastMarket.m_UpdateTime,this);
	//	}
	//}
	void Account::InitExchangeRule( const std::string& aConfigFile )
	{
		boost::property_tree::ptree ptExchange;
		read_xml(aConfigFile,ptExchange);
		m_ExechangeSetting.m_LimitMaxVol = ptExchange.get<int>("ExchangeRuluConfig.LimitMaxVol");
		m_ExechangeSetting.m_MarketMaxVol = ptExchange.get<int>("ExchangeRuluConfig.MarketMaxVol");
		m_ExechangeSetting.m_MaxCancleTime = ptExchange.get<int>("ExchangeRuluConfig.MaxCancleTimeVol");
		m_ExechangeSetting.m_TotalMaxOpenVol = ptExchange.get<int>("ExchangeRuluConfig.MaxTotalOpenVol");
		m_ExechangeSetting.m_SinglePositionVol = ptExchange.get<int>("ExchangeRuluConfig.SinglePositionMaxVol");
		m_ExechangeSetting.m_AutoTradeMaxTime = ptExchange.get<int>("ExchangeRuluConfig.AutoTradeMaxTime");
		RestoreTradeVol();
	}
	void Account::SetSignalDirectionVol(BuySellType type,int Vol,bool bAdd)
	{
		if(type == BuySellType::BuyOrder)
		{
			if(bAdd)
			{
				m_ExechangeSetting.m_BuyDirectionVol += Vol;
			}
			else
			{
				m_ExechangeSetting.m_BuyDirectionVol -= Vol;
			}
		}
		else
		{
			if (bAdd)
			{
				m_ExechangeSetting.m_SellDirectionVol += Vol;
			}
			else
			{
				m_ExechangeSetting.m_SellDirectionVol -= Vol;
			}
		}
		StoreTradeVol();
	}
	void Account::StoreTradeVol()
	{
		std::shared_ptr<AT::TradeVolData> pTradeVolData(new TradeVolData);
		pTradeVolData->BuyDirectionVol = m_ExechangeSetting.m_BuyDirectionVol;
		pTradeVolData->SellDirectionVol = m_ExechangeSetting.m_SellDirectionVol;
		pTradeVolData->TotalCancleTime = m_ExechangeSetting.m_TotalCancleTime;
		pTradeVolData->TotalOpenVol = m_ExechangeSetting.m_TotalOpenVol;
		pTradeVolData->AutoTradeTime = m_ExechangeSetting.m_AutoTradeTime;
		m_TradeVolDB->StoreTradeVolData(pTradeVolData);
	}
	void Account::RestoreTradeVol()
	{
		std::shared_ptr<TradeVolMap> lpTradeVolMap(new TradeVolMap);
		m_TradeVolDB->RestoreTradeVolData(lpTradeVolMap);
		if(lpTradeVolMap->size() > 0)
		{
			m_ExechangeSetting.m_BuyDirectionVol = lpTradeVolMap->rbegin()->second->BuyDirectionVol;
			m_ExechangeSetting.m_SellDirectionVol = lpTradeVolMap->rbegin()->second->SellDirectionVol;
			m_ExechangeSetting.m_TotalCancleTime = lpTradeVolMap->rbegin()->second->TotalCancleTime;
			m_ExechangeSetting.m_TotalOpenVol = lpTradeVolMap->rbegin()->second->TotalOpenVol;
			m_ExechangeSetting.m_AutoTradeTime = lpTradeVolMap->rbegin()->second->AutoTradeTime;
			return;	
		}
		
		m_ExechangeSetting.m_BuyDirectionVol = 0;
		m_ExechangeSetting.m_SellDirectionVol = 0;
		m_ExechangeSetting.m_TotalCancleTime = 0;
		m_ExechangeSetting.m_TotalOpenVol = 0;
		m_ExechangeSetting.m_AutoTradeTime = 0;
	}

	void Account::InitExchangeStroe()
	{
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
	}

	void Account::InitExecutorContainer( const std::string& aExecutorConfigFile )
	{
			m_pExecutorContianer.reset(new ExecutorContianer(aExecutorConfigFile));
	}

}