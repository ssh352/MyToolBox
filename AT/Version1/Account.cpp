#include "Account.h"
#include "IExecutor.h"
#include "IDriver_TD.h"


#include "ExecutorFactory.h"
#include "ATLogger.h"

#include <boost\bind.hpp>
#include <boost\property_tree\ptree.hpp>
#include <boost\property_tree\xml_parser.hpp>
#include <boost\filesystem.hpp>
#include <boost\date_time\gregorian\conversion.hpp>

namespace AT
{

	Account::Account( const std::string& aConfigFile, IDriver_TD* apTD )
		:m_pTD(apTD)
		
	{
		m_Status = AccountStatus::IDLE;
		m_PorfitOfCurrentSignal = 0;
		InitFromConfigFile(aConfigFile);


		m_FellowCloseTime = boost::posix_time::microsec_clock::local_time();

		boost::gregorian::date lToday = boost::gregorian::day_clock::local_day();
		m_FellowCloseTime = boost::posix_time::ptime(lToday,boost::posix_time::duration_from_string("15:06:00"));
		//m_MarketCloseTime =  ("15:08:00");
		

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
		m_ExchangePath = lpt.get<std::string>("AccountFile.ExchangSavePath");
		InitExchangeStroe();

		std::string lDispatcherConfig =  lpt.get<std::string>("AccountFile.SignalHandleSet");
		InitSignalDispatcher(lDispatcherConfig);

		std::string lFliterFile = lpt.get<std::string>("AccountFile.FilterConfig");
		InitFliterConfig(lFliterFile);
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

		if (m_Status == AccountStatus::OnSignal)
		{
			if(aMarketDepth.m_UpdateTime > m_FellowCloseTime)
			{
				m_CurrentExecutor->Abrot();
				m_Status = AccountStatus::WaittingForAbort;
			}
		}


		m_LastTime = aMarketDepth.m_UpdateTime;
		if(m_CurrentExecutor != nullptr )
		{
			m_CurrentExecutor->OnMarketDepth(aMarketDepth);
		}
		
	}

	void Account::OnRtnOrder( const OrderUpdate& apOrder )
	{
		
		CheckCancelTimes(apOrder);


		m_CurrentExecutor->OnRtnOrder(apOrder);


		switch (m_Status)
		{
		case AT::Account::AccountStatus::IDLE:
			break;
		case AT::Account::AccountStatus::OnSignal:
			if (m_CurrentExecutor->GetExecutionStatus().IsFinised)
			{
				m_Status = AccountStatus::IDLE;
				m_ProfitStatusMap[apOrder.m_UpdateTime] =  m_PorfitOfCurrentSignal/m_AccountVol ;
				ATLOG(L_INFO,"Signal Finished Profit %d",m_PorfitOfCurrentSignal/m_AccountVol);
				m_PorfitOfCurrentSignal  = 0;

			}
			break;
		case AT::Account::AccountStatus::WaittingForAbort:
			if (m_CurrentExecutor->GetExecutionStatus().IsFinised)
			{
				m_Status = AccountStatus::OnCloseStatus_Fellow;

				for (auto lPositon: m_PositionMap)
				{

					if (lPositon.second.BuyPosition > 0)
					{
						AT::ExecutorInput lCloseInput;
						strcpy_s(lCloseInput.InstrumentID ,cInstrimentIDLength,lPositon.first.c_str());
						lCloseInput.BuySellCode = BuySellType::SellOrder;
						lCloseInput.OpenCloseCode = OpenCloseType::CloseToday;
						boost::shared_ptr<IExecutor> lCloseExecutor  = ExecutorFactory::CreateExecutor( "Follow","FollowExecutor.xml");
						lCloseExecutor->AddExecution(lCloseInput);
						m_CloseExecutors.push_back(lCloseExecutor);
					}

					if (lPositon.second.SellPosition > 0)
					{
						AT::ExecutorInput lCloseInput;
						strcpy_s(lCloseInput.InstrumentID ,cInstrimentIDLength,lPositon.first.c_str());
						lCloseInput.BuySellCode = BuySellType::BuyOrder;
						lCloseInput.OpenCloseCode = OpenCloseType::CloseToday;
						boost::shared_ptr<IExecutor> lCloseExecutor = ExecutorFactory::CreateExecutor( "Follow","FollowExecutor.xml");
						lCloseExecutor->AddExecution(lCloseInput);
						m_CloseExecutors.push_back(lCloseExecutor);
					}
			
				}
		
	
			}
			break;
		case AT::Account::AccountStatus::OnCloseStatus_Fellow:
			{

				for (auto eachCloseExecutor:m_CloseExecutors)
				{
					eachCloseExecutor->OnRtnOrder(apOrder);
				}

				bool isAllDOne = true;
				for (auto eachCloseExecutor:m_CloseExecutors)
				{
					if(eachCloseExecutor->GetExecutionStatus().IsFinised)
					{
						isAllDOne  = false;
					}
				}

				if(isAllDOne)
				{
					m_Status = AccountStatus::Done;
				}
			}
			break;
		case AT::Account::AccountStatus::OnCloseStatus_Market:
			break;
		case AT::Account::AccountStatus::Done:
			break;
		default:
			break;
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

		UpdatePositionMap(aTrade);

		
		
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
			m_StoreExchangeRule.TotalCancleTime++;
			if(m_StoreExchangeRule.TotalCancleTime>m_ExchangeRule.MaxCancleTimeVol)
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

	void Account::UpdatePositionMap( ExecutionResult &aTrade )
	{
		if(aTrade.IsBuy == BuySellType::BuyOrder)
		{
			int& Position = m_PositionMap[aTrade.InstrumentID].BuyPosition;
			if (aTrade.IsOpen == OpenCloseType::Open)
			{
				Position += aTrade.vol;
			}
			else
			{
				Position -= aTrade.vol;
			}
		}
		else
		{
			int& Position = m_PositionMap[aTrade.InstrumentID].SellPosition;
			if (aTrade.IsOpen == OpenCloseType::Open)
			{
				Position += aTrade.vol;
			}
			else
			{
				Position -= aTrade.vol;
			}
		}
	}
	void Account::InitFliterConfig(const std::string& aFliterConfig)
	{
		boost::property_tree::ptree lpt;
		read_xml(aFliterConfig,lpt);

		FliterStruct fliter;
		int iID;
		int iTime;
		std::string strStopTime;
		std::string strStartTime;

		m_FilterSetting.Time1 = lpt.get<int>("Time1");
		m_FilterSetting.Time2 = lpt.get<int>("Time2");
		m_FilterSetting.Time3 = lpt.get<int>("Time3");
		iTime = lpt.get<int>("TotalTimeProfit.Time");
		m_FilterSetting.TotalTimeProfit[iTime] = lpt.get<int>("TotalTimeProfit.Profit");
		m_FilterSetting.TotalProfitStop = lpt.get<int>("TotalProfitStop");
		strStopTime = to_simple_string(boost::gregorian::day_clock::local_day())+" "+lpt.get<std::string>("SignalFliter.Item.StopTime");
		m_FilterSetting.StopTime = boost::posix_time::time_from_string(strStopTime);
		strStartTime = to_simple_string(boost::gregorian::day_clock::local_day())+" "+lpt.get<std::string>("SignalFliter.Item.StartTime");
		m_FilterSetting.StartTime = boost::posix_time::time_from_string(strStartTime);
	}
	void Account::StoreTradeVol()
	{
		std::shared_ptr<AT::TradeVolData> pTradeVolData(new TradeVolData);
		pTradeVolData->BuyDirectionVol = m_StoreExchangeRule.BuyDirectionVol;
		pTradeVolData->SellDirectionVol = m_StoreExchangeRule.SellDirectionVol;
		pTradeVolData->TotalCancleTime = m_StoreExchangeRule.TotalCancleTime;
		pTradeVolData->TotalOpenVol = m_StoreExchangeRule.TotalOpenVol;
		pTradeVolData->AutoTradeTime = m_StoreExchangeRule.AutoTradeTime;
		m_TradeVolDB->StoreTradeVolData(pTradeVolData);
	}
	void Account::RestoreTradeVol()
	{
		std::shared_ptr<TradeVolMap> lpTradeVolMap(new TradeVolMap);
		m_TradeVolDB->RestoreTradeVolData(lpTradeVolMap);
		if(lpTradeVolMap->size() > 0)
		{
			m_StoreExchangeRule.BuyDirectionVol = lpTradeVolMap->rbegin()->second->BuyDirectionVol;
			m_StoreExchangeRule.SellDirectionVol = lpTradeVolMap->rbegin()->second->SellDirectionVol;
			m_StoreExchangeRule.TotalCancleTime = lpTradeVolMap->rbegin()->second->TotalCancleTime;
			m_StoreExchangeRule.TotalOpenVol = lpTradeVolMap->rbegin()->second->TotalOpenVol;
			m_StoreExchangeRule.AutoTradeTime = lpTradeVolMap->rbegin()->second->AutoTradeTime;
			return;	
		}

		m_StoreExchangeRule.BuyDirectionVol = 0;
		m_StoreExchangeRule.SellDirectionVol = 0;
		m_StoreExchangeRule.TotalCancleTime = 0;
		m_StoreExchangeRule.TotalOpenVol = 0;
		m_StoreExchangeRule.AutoTradeTime = 0;
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
}