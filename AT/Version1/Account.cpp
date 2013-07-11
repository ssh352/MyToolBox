#include "Account.h"
#include "IExecutor.h"
#include "IDriver_TD.h"

#include "ExecutorContianer.h"
#include "AccountSignalDispach.h"
#include "ATLogger.h"
#include "ISignalFliter.h"

#include <boost\bind.hpp>
#include <boost\property_tree\ptree.hpp>
#include <boost\property_tree\xml_parser.hpp>
#include <boost\filesystem.hpp>
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

		std::string LExecutorContainerConfig =  lpt.get<std::string>("AccountFile.ExecutorConfig");
		InitExecutorContainer(LExecutorContainerConfig);

		std::string lDispatcherConfig =  lpt.get<std::string>("AccountFile.SignalHandleSet");

		InitSignalDispatcher(lDispatcherConfig);

	}	

	Account::~Account(void)
	{
	}


	void Account::InitExecutorContainer( const std::string& aExecutorConfigFile )
	{
		CommandHandler lpCommandHandle = boost::bind(&Account::HandleCommand,this,_1);
		TradeReportFun lpTradeReportFun = boost::bind(&Account::HandleExecutorResult,this,_1);
		m_pExecutorContianer.reset(new ExecutorContianer(aExecutorConfigFile,lpCommandHandle,lpTradeReportFun));
	}

	void Account::HandleTradeSignal( const Signal& aTradeSignal )
	{
		//m_pFilter-(aTradeSignal);
		m_pAccountSignalDispach->HandleSignal(aTradeSignal);
	}

	void Account::OnMarketDepth( const MarketData& aMarketDepth )
	{
		m_pExecutorContianer->OnMarketDepth(aMarketDepth);
	}

	void Account::OnRtnOrder( const OrderUpdate& apOrder )
	{
		m_pExecutorContianer->OnRtnOrder(apOrder);
	}

	void Account::OnRtnTrade( const TradeUpdate& apTrade )
	{
		m_pExecutorContianer->OnRtnTrade(apTrade);
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

	void Account::HandleExecutorResult( ExecutionResult )
	{
		//todo update
		m_pFilter;
	}

	void Account::InitSignalDispatcher( const std::string& aDispatcherConfigFile )
	{
		m_pAccountSignalDispach.reset(new AccountSignalDispach(aDispatcherConfigFile,m_pExecutorContianer.get(),m_AccountVol));
	}

}