#include "CTP_MULTI_TD.h"
#include "CTP_TD.h"
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>
#include <set>
#include <string>
namespace CTP
{


	CTP_MULTI_TD::CTP_MULTI_TD(void)
	{
	}


	CTP_MULTI_TD::~CTP_MULTI_TD(void)
	{
	}

	void CTP_MULTI_TD::Init( const std::map<std::string,std::string>& aConfigMap, AT::ITradeSpi* apTradeSpi )
	{
		boost::property_tree::ptree lMultiTDpt;
		std::string ConfigFileName =aConfigMap.at("ConfigFile");
		read_xml(ConfigFileName,lMultiTDpt);
		std::set<std::string> lTDCOnfigList;
		for(auto lVale: lMultiTDpt.get_child("Multi_Config.Itemlist"))
		{
			lTDCOnfigList.insert(lVale.second.data());
		}
		
		for(std::string lConfigName :lTDCOnfigList )
		{
			std::map<std::string,std::string> lTDConfigMap;
			std::string lConfigNodeName ="Multi_Config."+lConfigName + '.';
			lTDConfigMap["BrokerID"] = lMultiTDpt.get<std::string>(lConfigNodeName+"BrokerID");
			lTDConfigMap["UserID"] =  lMultiTDpt.get<std::string>(lConfigNodeName+"UserID");
			lTDConfigMap["Password"]= lMultiTDpt.get<std::string>(lConfigNodeName+"Password");
			lTDConfigMap["Front"] = lMultiTDpt.get<std::string>(lConfigNodeName+"Front");
			lTDConfigMap["StoreDir"] = lMultiTDpt.get<std::string>(lConfigNodeName+"StoreDir");
			std::shared_ptr<CTP_TD> lCurrentTDInst(new CTP_TD);
			lCurrentTDInst->Init(lTDConfigMap,apTradeSpi);
			m_TradeInstMap[lConfigName] =lCurrentTDInst;
		}

	}

	std::string CTP_MULTI_TD::CreateOrder( const std::string& aNewOrder )
	{
		std::stringstream lbuf(aNewOrder);
		using boost::property_tree::ptree;
		ptree pt;
		read_xml(lbuf,pt);

		std::string lConfigName = pt.get<std::string>("Order.User");

		std::string lClientOrderID = m_TradeInstMap[lConfigName]->CreateOrder(aNewOrder);
		mCLientID_TD_Map[lClientOrderID] = m_TradeInstMap[lConfigName];
		return lClientOrderID;
		
	}

	void CTP_MULTI_TD::DeleteOrder( const std::string& aClientOrderID )
	{
		 mCLientID_TD_Map[aClientOrderID]->DeleteOrder(aClientOrderID);
	}

	void CTP_MULTI_TD::ModifyOrder( const std::string& aRequest )
	{
		return;
	}

	void CTP_MULTI_TD::QueryPosition( const std::string& aRequest )
	{
		//todo
		return;
	}

}
