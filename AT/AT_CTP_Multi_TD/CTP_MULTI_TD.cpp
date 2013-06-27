#include "CTP_MULTI_TD.h"
#include "CTP_TD.h"
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>
#include <set>
#include <string>
namespace CTP
{


	CTP_MULTI_TD::CTP_MULTI_TD(const std::string& aConfigFile,AT::ITradeSpi* apTradeSpi)
		:m_pTradeSpi(apTradeSpi)
	{

		std::map<std::string,std::string> lCTP_TD_Map;
		boost::property_tree::ptree lpt;
		read_xml(aConfigFile,lpt);

		for(auto lSignalAccountNodePair: lpt.get_child("Multi_Config"))
		{
			std::string lAccountID = lSignalAccountNodePair.second.get<std::string>("AccountID");
			std::string lAccountConfig = lSignalAccountNodePair.second.get<std::string>("Config");
			lCTP_TD_Map[lAccountID] = lAccountConfig;
		}

		for (auto lSingleTDPair: lCTP_TD_Map)
		{
			LoadSignalTD(lSingleTDPair.first,lSingleTDPair.second);
			//m_TradeInstMap[lSingleTDPair.first].reset(new CTP_TD(lSingleTDPair.second.c_str(),m_pTradeSpi));
		}
	}


	CTP_MULTI_TD::~CTP_MULTI_TD(void)
	{
	}



	void CTP_MULTI_TD::LoadSignalTD( const std::string& AccountName, const std::string aConfigFileName )
	{
		m_TradeInstMap[AccountName].reset(new CTP_TD(aConfigFileName.c_str(),m_pTradeSpi));
	}

	void CTP_MULTI_TD::UpdateParam( const AT::Param& apParam )
	{
		//nothing todo now;
	}

	void CTP_MULTI_TD::Start()
	{
		for (auto lTDInstPair: m_TradeInstMap)
		{
			lTDInstPair.second->Start();
		}
		
	}

	void CTP_MULTI_TD::Stop()
	{
		for (auto lTDInstPair: m_TradeInstMap)
		{
			lTDInstPair.second->Stop();
		}
	}

	void CTP_MULTI_TD::CreateOrder( const AT::InputOrder& aNewOrder )
	{
		std::string lAccountID = aNewOrder.AccoutID;
		m_TradeInstMap[lAccountID]->CreateOrder(aNewOrder);
		//保存Key 与Account之间的映射，当撤单的时候能够正确找到原来的账户
		m_OrderKeyMap[lAccountID].insert(aNewOrder.m_Key);
	}

	void CTP_MULTI_TD::DeleteOrder( const AT::CancelOrder& aDelOrderID )
	{
		std::string lAccountID = FindOrderInstByOrderKey(aDelOrderID.m_Key);
		m_TradeInstMap[lAccountID]->DeleteOrder(aDelOrderID);
		return;
	}

	void CTP_MULTI_TD::ModifyOrder( const AT::ModifyOrder& aRequest )
	{
		std::string lAccountID = FindOrderInstByOrderKey(aRequest.m_Key);
		m_TradeInstMap[lAccountID]->ModifyOrder(aRequest);
		return;
	}

	std::string CTP_MULTI_TD::FindOrderInstByOrderKey( const AT::AT_Order_Key &aKEy )
	{
		for(auto lOrderKeyMapPair:m_OrderKeyMap)
		{
			std::set<AT::AT_Order_Key>& lOrderKeySet = lOrderKeyMapPair.second;
			if (lOrderKeySet.find(aKEy) !=lOrderKeySet.end() )
			{
				return  lOrderKeyMapPair.first;
			}
		}
		return "";
	}

}
