#include "CTP_TD.h"
#include "CTP_MULTI_TD.h"
#include <boost/thread.hpp>
#include <boost/date_time.hpp>
#include "PrintTDSpi.h"
#include <iostream>
#include <fstream>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>
#include "CTP_TD_Mock.h"

int main()
{
	PrintTDSpi lSpi;

	//CTP::CTP_MULTI_TD lInst;
	CTP::CTP_TD_Mock  lInst;
	std::map<std::string,std::string> lConfig;
	//lConfig["ConfigFile"] = "C:\\GitTrunk\\MyToolBox\\AT\\AT_CTP_Multi_TD\\Multi_TD.xml";
	lInst.Init(lConfig,&lSpi);

	char buf;
	while(std::cin>>buf)
	{
		if(buf == '1')
		{
			using boost::property_tree::ptree;
			ptree pt;
			pt.put("head.type","PlaceOrder");
			pt.put("head.version",0.1f);
			pt.put("Order.User","TDConfig1");
			pt.put("Order.ID" , "IF1304");
			pt.put("Order.BuyCode" , "Sell");
			pt.put("Order.OpenCode" , "Open");
			pt.put("Order.Price" , "2600" );
			pt.put("Order.Vol" , 1 );
			std::stringstream lStringStream;
			write_xml(lStringStream,pt);
			lInst.CreateOrder(lStringStream.str());
		}

		if(buf == '2')
		{
			using boost::property_tree::ptree;
			ptree pt;
			pt.put("head.type","PlaceOrder");
			pt.put("head.version",0.1f);
			pt.put("Order.User","TDConfig2");
			pt.put("Order.ID" , "IF1304");
			pt.put("Order.BuyCode" , "Sell");
			pt.put("Order.OpenCode" , "Open");
			pt.put("Order.Price" , "2600" );
			pt.put("Order.Vol" , 1 );
			std::stringstream lStringStream;
			write_xml(lStringStream,pt);
			lInst.CreateOrder(lStringStream.str());
		}
	}

}