#include "Account.h"
#include "IExecutor.h"
#include "IDriver_TD.h"


#include "ExecutorFactory.h"
#include "ATLogger.h"

#include <boost\bind.hpp>
#include <boost\property_tree\ptree.hpp>
#include <boost\property_tree\xml_parser.hpp>
#include <boost\filesystem.hpp>

namespace AT
{

	Account::Account( const std::string& aConfigFile, IDriver_TD* apTD )
		:m_pTD(apTD)
		
	{
		m_Status = AccountStatus::IDLE;
		m_PorfitOfCurrentSignal = 0;
		InitFromConfigFile(aConfigFile);

		//todo save and load ExechangRule
		m_CancelTimes = 0;
		// 
		

	}

	void Account::InitFromConfigFile( const std::string& aConfigFile )
	{
		boost::property_tree::ptree lpt;
		read_xml(aConfigFile,lpt);

		m_AccountID = lpt.get<std::string>("AccountFile.AccountID");
		std::string lExchangRuleFile =  lpt.get<std::string>("AccountFile.ExchangeRule");
		InitExechangeRule(lExchangRuleFile);

		std::string LExecutorContainerConfig =  lpt.get<std::string>("AccountFile.ExecutorConfig");
		InitExecutorContainer(LExecutorContainerConfig);

		m_AccountVol = lpt.get<int>("AccountFile.TargetVol");

		std::string lDispatcherConfig =  lpt.get<std::string>("AccountFile.SignalHandleSet");
		InitSignalDispatcher(lDispatcherConfig);
	}	

	Account::~Account(void)
	{
	}


	void Account::InitExecutorContainer( const std::string& aExecutorConfigFile )
	{

		boost::property_tree::ptree lConfig;
		read_xml(aExecutorConfigFile,lConfig);
		for( std::pair<std::string,boost::property_tree::ptree>  lExecutorItem : lConfig.get_child("Executors"))
		{
			std::string lExecuorID =  lExecutorItem.second.get<std::string>("ExecutorID");
			std::string lExecutorType = lExecutorItem.second.get<std::string>("ExecutorType");
			std::string lExecuorConfig =  lExecutorItem.second.get<std::string>("ExectorConfigFile");
			ExecutorPara lPara = {lExecutorType,lExecuorConfig};
			m_HandleInfoGroup.insert(make_pair( lExecuorID,lPara ));
			//boost::shared_ptr<IExecutor> lExecutor = ExecutorFactory::CreateExecutor(lExecutorType,lExecuorConfig);
			//lExecutor->SetCommandHandler(m_CommandHandler);
			//lExecutor->SetTradeReportCallback(m_ReportFun);
			//m_ExecutorGruop.insert(make_pair(lExecutor->GetExecutorID(),lExecutor));
		}


		//m_pExecutorContianer.reset(new ExecutorContianer(aExecutorConfigFile,lpCommandHandle,lpTradeReportFun));
	}

	void Account::InitSignalDispatcher( const std::string& aDispatcherConfigFile )
	{
		boost::property_tree::ptree lConfig;
		read_xml(aDispatcherConfigFile,lConfig);
		for( std::pair<std::string,boost::property_tree::ptree>  lSignalHandleItem : lConfig.get_child("SignalDispatch"))
		{

			std::string lSignalID = lSignalHandleItem.second.get<std::string>("SignalID");
			std::string lBuySellString = lSignalHandleItem.second.get<std::string>("BuySellCode");
			std::string lOpenCloseString = lSignalHandleItem.second.get<std::string>("OpenCloseCode");
			std::string lInstrumentID =  lSignalHandleItem.second.get<std::string>("InstrumentID");
			std::string lExecutorID = lSignalHandleItem.second.get<std::string>("ExecutorID");

			SignalExecutInfo lOneSignalCase;
			lOneSignalCase.BuySellCode = lBuySellString=="Buy" ? BuySellType::BuyOrder : BuySellType::SellOrder;
			lOneSignalCase.OpenCloseCode = lOpenCloseString == "Open" ? OpenCloseType::Open : OpenCloseType::Close;
			lOneSignalCase.ExecutorID = lExecutorID;
			lOneSignalCase.InstrumentID = lInstrumentID;
			lOneSignalCase.vol = m_AccountVol;

			m_Signal_Executor_Map[lSignalID] = lOneSignalCase;
		}
	}

	void Account::HandleTradeSignal( const Signal& aSignal )
	{
		
		TryToFilter();
		return;




		switch (m_Status)
		{
		case AccountStatus::IDLE:
			DoNewSignal(aSignal);

			break;
		case AccountStatus::OnSignal:
			break;
		case AccountStatus::OnCloseStatus_Fellow:
			break;
		case AccountStatus::OnCloseStatus_Market:
			break;
		default:
			break;
		}
	}

	void Account::OnMarketDepth( const MarketData& aMarketDepth )
	{
		m_LastTime = aMarketDepth.m_UpdateTime;
		m_CurrentExecutor->OnMarketDepth(aMarketDepth);
	}

	void Account::OnRtnOrder( const OrderUpdate& apOrder )
	{
		
		CheckCancelTimes(apOrder);


		m_CurrentExecutor->OnRtnOrder(apOrder);


		if (m_CurrentExecutor->GetExecutionStatus().IsFinised)
		{
			m_Status = AccountStatus::IDLE;
			m_ProfitStatusMap[apOrder.m_UpdateTime] =  m_PorfitOfCurrentSignal/m_AccountVol ;
			ATLOG(L_INFO,"Signal Finished Profit %d",m_PorfitOfCurrentSignal/m_AccountVol);
			m_PorfitOfCurrentSignal  = 0;
			
		}
	}

	void Account::OnRtnTrade( const TradeUpdate& apTrade )
	{
		m_CurrentExecutor->OnRtnTrade(apTrade);
	}


	void Account::HandleCommand( Command apTradeCommand )
	{
		switch (apTradeCommand.m_CommandType)
		{
		case CommandType::Invalid:
			return;
		case CommandType::Input:
			{
				strcpy_s(apTradeCommand.m_InputOrder.AccoutID,cAccoutIDLength,m_AccountID.c_str());

				if (apTradeCommand.m_InputOrder.m_Vol > m_ExchangeRule.LimitMaxVol && apTradeCommand.m_InputOrder.m_OrderPriceType == OrderPriceType::LimitPrice)
				{
					ATLOG(L_ERROR,"Limit Order Execute max vol");
				}
				if (apTradeCommand.m_InputOrder.m_Vol > m_ExchangeRule.MarketMaxVol && apTradeCommand.m_InputOrder.m_OrderPriceType == OrderPriceType::MarketPrice)
				{
					ATLOG(L_ERROR,"Market Order Execute max vol");
				}
				m_OrderKeyList.insert(apTradeCommand.m_InputOrder.m_Key);
				m_pTD->CreateOrder(apTradeCommand.m_InputOrder);
			}
			break;
		case CommandType::Modify:
			{
				m_pTD->ModifyOrder(apTradeCommand.m_ModifyOrder);
			}
			break;			
		case CommandType::Cancel:
			{
				m_pTD->DeleteOrder(apTradeCommand.m_CancelOrder);
			}
			break;	
		default:
			break;
		}
	}

	void Account::HandleExecutorResult( ExecutionResult aTrade )
	{
		if(aTrade.IsBuy == BuySellType::BuyOrder)
		{
			m_PorfitOfCurrentSignal -= aTrade.Price* aTrade.vol;
		}
		else
		{
			m_PorfitOfCurrentSignal += aTrade.Price* aTrade.vol;
		}
		
	}

	void Account::InitExechangeRule( const std::string& aConfig )
	{
		boost::property_tree::ptree lpt;
		read_xml(aConfig,lpt);
		m_ExchangeRule.AutoTradeMaxTime = lpt.get<int>("ExchangeRuluConfig.AutoTradeMaxTime");
		m_ExchangeRule.LimitMaxVol = lpt.get<int>("ExchangeRuluConfig.LimitMaxVol");
		m_ExchangeRule.MarketMaxVol = lpt.get<int>("ExchangeRuluConfig.MarketMaxVol");
		m_ExchangeRule.SinglePositionMaxVol = lpt.get<int>("ExchangeRuluConfig.SinglePositionMaxVol");
		m_ExchangeRule.MaxTotalOpenVol = lpt.get<int>("ExchangeRuluConfig.MaxTotalOpenVol");
		m_ExchangeRule.MaxCancleTimeVol = lpt.get<int>("ExchangeRuluConfig.MaxCancleTimeVol");
	}

	void Account::DoNewSignal( const Signal &aSignal )
	{
		if (!TryToFilter())
		{
			return;
		}

		if(m_Signal_Executor_Map.find(aSignal.m_ID) == m_Signal_Executor_Map.end())
		{
			ATLOG(L_ERROR,"No Handle Signal, because did not find the Executor in the map");
			return;
		}
		const SignalExecutInfo& lExecutInfo =  m_Signal_Executor_Map[aSignal.m_ID];
		const std::string& lExectorID = lExecutInfo.ExecutorID;
		if (m_HandleInfoGroup.find(lExectorID) == m_HandleInfoGroup.end())
		{
			ATLOG(L_ERROR,"No Handle Signal , because Not Find Executor In Executor Config map");
			return;
		}

		m_CurrentExecutor = ExecutorFactory::CreateExecutor(m_HandleInfoGroup[lExectorID].Type,m_HandleInfoGroup[lExectorID].ConfigFile);
		CommandHandler lpCommandHandle = boost::bind(&Account::HandleCommand,this,_1);
		TradeReportFun lpTradeReportFun = boost::bind(&Account::HandleExecutorResult,this,_1);
		m_CurrentExecutor->SetCommandHandler(lpCommandHandle);
		m_CurrentExecutor->SetTradeReportCallback(lpTradeReportFun);

		ExecutorInput lret;
		strcpy_s(lret.InstrumentID,cInstrimentIDLength,lExecutInfo.InstrumentID.c_str());
		lret.BuySellCode = lExecutInfo.BuySellCode;
		lret.OpenCloseCode = lExecutInfo.OpenCloseCode;
		lret.vol = m_AccountVol;
		lret.TriggerMarketData = aSignal.m_TriggerMarketData;

		m_CurrentExecutor->AddExecution(lret);
		m_Status = AccountStatus::OnSignal;
	}

	void Account::CheckCancelTimes( const OrderUpdate &apOrder )
	{
		if(apOrder.m_OrderStatus == OrderStatusType::Canceled)
		{
			m_CancelTimes ++;
			if(m_CancelTimes>m_ExchangeRule.MaxCancleTimeVol)
			{
				ATLOG(L_ERROR,"Max Cancel Time over");
			}
		}
	}


	Account::LastNTradeStatus Account::IsLastNTradeLoss( int lastI )
	{
		LastNTradeStatus lret = {false,0};
		std::map<AT_Time,int32_t>::reverse_iterator iter = m_ProfitStatusMap.rbegin();
		for (int i = 0; i< lastI;++i)
		{
			if(iter!= m_ProfitStatusMap.rend() && iter->second < 0 )
			{
				lret.m_TotalLoss += iter->second;
				lret.m_TimeLoss = iter->first;
			}
			else
			{
				lret.m_isAllLoss = false;
				return lret;
			}
			iter--;
		}
		lret.m_isAllLoss = true;
		return lret;
	}

	bool Account::TryToFilter()
	{

		return true;
		LastNTradeStatus lret1 = IsLastNTradeLoss(1);
		if(m_ProfitStatusMap.size() > 0)
		{
			if(lret1.m_isAllLoss && (m_LastTime - m_ProfitStatusMap.rbegin()->first) < boost::posix_time::seconds(m_FilterSetting.Time1) )
			{
				return false;
			}
		}

		//2次

		LastNTradeStatus lret2 = IsLastNTradeLoss(2);
		if(lret2.m_isAllLoss && (m_LastTime-lret2.m_TimeLoss)<boost::posix_time::seconds(m_FilterSetting.Time2))
		{

			return false;
		}


		LastNTradeStatus lret3 = IsLastNTradeLoss(3);
		if(lret3.m_isAllLoss && (m_LastTime-lret3.m_TimeLoss)<boost::posix_time::seconds(m_FilterSetting.Time3))
		{
			return false;
		}
		//3次

		//总亏损
		int totalProfit = 0;
		for (auto lProfitPair:m_ProfitStatusMap)
		{
			totalProfit += lProfitPair.second;
		}
		if(totalProfit <m_FilterSetting.TotalProfitStop)
		{
			return false;
		}

		//总亏损N点时N秒冻结时长
		if(m_ProfitStatusMap.size() > 0 )
		{
			if(totalProfit <m_FilterSetting.TotalTimeProfit.begin()->first &&
				(m_LastTime - m_ProfitStatusMap.rbegin()->first) < boost::posix_time::seconds(m_FilterSetting.TotalTimeProfit.begin()->second))
			{
				return false;
			}
		}

		return true;
	}

}