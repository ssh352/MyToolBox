#include "TradeAccountContainer.h"
#include "IDriver_TD.h"
#include "TradeAccountDemo1.h"
#include <boost\bind.hpp>
#include <boost\function.hpp>
#include "../AT_Driver/ATLogger.h"
#include "boost/format.hpp"
#include "boost/property_tree/ptree.hpp"
#include "boost/property_tree/xml_parser.hpp"
namespace AT
{


TradeAccountContainer::TradeAccountContainer( const char* configFile,  AT::IDriver_TD* apTD )
{

	//todo load from file but now , just hard code
	AccountProfitStatus lnewStatus ;
	memset(&lnewStatus,0,sizeof(AccountProfitStatus));

	boost::property_tree::ptree pt;
	read_xml(configFile,pt);
	for (auto lAccount:pt.get_child("MultiAccount_Config"))
	{
		int lIsUse = lAccount.second.get<int>("IsUse");
		if(lIsUse == 1)
		{
			std::string lAccountID = lAccount.second.get<std::string>("AccountID");
			std::string lAccountConfig = lAccount.second.get<std::string>("Config");
			boost::shared_ptr<ITradeAccount> lpAccount;
			lpAccount.reset(new TradeAccountDemo1(lAccountConfig,apTD));
			m_AccountList.push_back(lpAccount);
			m_AccountFinishedList[lpAccount] = lnewStatus;
		}
	}
	

	boost::function< void(int32_t aProfit,AT_Time aTime ,ITradeAccount* sender)> lNotifyCallback
		= boost::bind(&TradeAccountContainer::HandleOneAccountProfit,this,_1,_2,_3);
	for (boost::shared_ptr<ITradeAccount> lAccoutPtr : m_AccountList)
	{
		lAccoutPtr->SetProfitCallback(lNotifyCallback);
	}
}


TradeAccountContainer::~TradeAccountContainer(void)
{
}

void TradeAccountContainer::OnRtnOrder( const OrderUpdate& apOrder )
{
	for (boost::shared_ptr<ITradeAccount> lAccoutPtr : m_AccountList)
	{
		lAccoutPtr->OnRtnOrder(apOrder);
	}
}

void TradeAccountContainer::OnRtnTrade( const TradeUpdate& apTrade )
{
	for (boost::shared_ptr<ITradeAccount> lAccoutPtr : m_AccountList)
	{
		lAccoutPtr->OnRtnTrade(apTrade);
	}
}

void TradeAccountContainer::OnMarketDepth( const MarketData& aMarketDepth )
{
	for (boost::shared_ptr<ITradeAccount> lAccoutPtr : m_AccountList)
	{
		lAccoutPtr->OnMarketDepth(aMarketDepth);
	}
}



void TradeAccountContainer::HandleTradeSignal( const TradeSignal& aTradeSignal )
{
	for (boost::shared_ptr<ITradeAccount> lAccoutPtr : m_AccountList)
	{
		lAccoutPtr->HandleTradeSignal(aTradeSignal);
	}

	for (auto lFinishedPtrValType : m_AccountFinishedList)
	{
		lFinishedPtrValType.second.isFinished = false;
	}
}

void TradeAccountContainer::HandleOneAccountProfit( int32_t aProfit,AT_Time aTime ,ITradeAccount* sender )
{
	AccountProfitStatus lnewStatus = {true,aProfit,aTime};
	for (auto lFinishedPtrValType : m_AccountFinishedList)
	{
		if(lFinishedPtrValType.first.get() == sender)
		{
			m_AccountFinishedList[lFinishedPtrValType.first] = lnewStatus;
			break;
		}
	}
	int totalProfix = 0;
	for (auto lFinishedPtrValType : m_AccountFinishedList)
	{
		if(lFinishedPtrValType.second.isFinished == false)
		{
			return;
		}
		totalProfix += lFinishedPtrValType.second.m_Profit;
	}


	m_ProfitNotifyer(totalProfix/m_AccountFinishedList.size(),aTime);

}

}