#include "AccountContainer.h"
#include "IDriver_TD.h"
#include "Account.h"
#include <boost\bind.hpp>
#include <boost\function.hpp>
#include "ATLogger.h"
#include "boost/format.hpp"
#include "boost/property_tree/ptree.hpp"
#include "boost/property_tree/xml_parser.hpp"

#include "RecoverAccount.h"
namespace AT
{


AccountContainer::AccountContainer( const char* configFile,  AT::IDriver_TD* apTD )
	:m_pTD(apTD)
{

	//AccountProfitStatus lnewStatus ;
	//memset(&lnewStatus,0,sizeof(AccountProfitStatus));

	boost::property_tree::ptree pt;
	read_xml(configFile,pt);
	for (auto lAccount:pt.get_child("MultiAccount_Config"))
	{

			std::string lAccountType = lAccount.second.get<std::string>("AccountType");
			std::string lAccountConfig = lAccount.second.get<std::string>("Config");

			boost::shared_ptr<IAccount> lpAccount = CreateAccount(lAccountType,lAccountConfig);
			m_AccountList.push_back(lpAccount);
			//m_AccountFinishedList[lpAccount] = lnewStatus;
	}
	

	//boost::function< void(int32_t aProfit,AT_Time aTime ,IAccount* sender)> lNotifyCallback
	//	= boost::bind(&TradeAccountContainer::HandleOneAccountProfit,this,_1,_2,_3);
	//for (boost::shared_ptr<IAccount> lAccoutPtr : m_AccountList)
	//{
	//	lAccoutPtr->SetProfitCallback(lNotifyCallback);
	//}
}


AccountContainer::~AccountContainer(void)
{
}

void AccountContainer::OnRtnOrder( const OrderUpdate& apOrder )
{
	for (boost::shared_ptr<IAccount> lAccoutPtr : m_AccountList)
	{
		lAccoutPtr->OnRtnOrder(apOrder);
	}
}

void AccountContainer::OnRtnTrade( const TradeUpdate& apTrade )
{
	for (boost::shared_ptr<IAccount> lAccoutPtr : m_AccountList)
	{
		lAccoutPtr->OnRtnTrade(apTrade);
	}
}

void AccountContainer::OnMarketDepth( const MarketData& aMarketDepth )
{
	for (boost::shared_ptr<IAccount> lAccoutPtr : m_AccountList)
	{
		lAccoutPtr->OnMarketDepth(aMarketDepth);
	}
}



void AccountContainer::HandleTradeSignal( const Signal& aTradeSignal )
{
	for (boost::shared_ptr<IAccount> lAccoutPtr : m_AccountList)
	{
		lAccoutPtr->HandleTradeSignal(aTradeSignal);
	}

	//for (auto lFinishedPtrValType : m_AccountFinishedList)
	//{
	//	lFinishedPtrValType.second.isFinished = false;
	//}
}

//void AccountContainer::HandleOneAccountProfit( int32_t aProfit,AT_Time aTime ,IAccount* sender )
//{
//	AccountProfitStatus lnewStatus = {true,aProfit,aTime};
//	for (auto lFinishedPtrValType : m_AccountFinishedList)
//	{
//		if(lFinishedPtrValType.first.get() == sender)
//		{
//			m_AccountFinishedList[lFinishedPtrValType.first] = lnewStatus;
//			break;
//		}
//	}
//	int totalProfix = 0;
//	for (auto lFinishedPtrValType : m_AccountFinishedList)
//	{
//		if(lFinishedPtrValType.second.isFinished == false)
//		{
//			return;
//		}
//		totalProfix += lFinishedPtrValType.second.m_Profit;
//	}
//
//
//	m_ProfitNotifyer(totalProfix/m_AccountFinishedList.size(),aTime);
//
//}

boost::shared_ptr<AT::IAccount> AccountContainer::CreateAccount( const std::string& aAccountType, const std::string& aConfigFile )
{
	boost::shared_ptr<AT::IAccount> lret;
	if(aAccountType == "Normal")
	{
		lret.reset(new Account(aConfigFile,m_pTD));
	}
	else if(aAccountType == "Recored")
	{
		lret.reset(new RecoverAccount(aConfigFile));
	}
	return lret;
}

}