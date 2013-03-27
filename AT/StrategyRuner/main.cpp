#include <fstream>
#include <string>
#include <boost/thread.hpp>
#include <boost/date_time.hpp>

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>
#include "CTP_MD.h"
#include "CTP_TD.h"
#include "StrPoster.h"
#include "../MD_PrintTest/PrintMDSpi.h"
#include "myForeach.h"
#include <exception>
int main()
{
	
	try
	{
		std::map<std::string,std::string> lMDConfigMap;
		boost::property_tree::ptree lMDpt;
		read_xml("MDConfig.xml",lMDpt);
		lMDConfigMap["EnableStateReceiver"] =   lMDpt.get<std::string>("MDConfig.EnableStateReceiver");
		lMDConfigMap["EnableSubscribeList"] =  lMDpt.get<std::string>("MDConfig.EnableSubscribeList");
		lMDConfigMap["IsReplay"] =  lMDpt.get<std::string>("MDConfig.IsReplay");
		lMDConfigMap["Front"]  = lMDpt.get<std::string>("MDConfig.Front");
		std::string  lSubList;
		MYFOREACH(v, lMDpt.get_child("MDConfig.SubList"))
		{
			lSubList += (v.second.data());
			lSubList += " ";

		}
		lMDConfigMap["SubscribeList"] = lSubList;


		CTP::CTP_TD lTdInst;
		CTP::CTP_MD lMDinst;

		StrPoster lStrRunner(&lTdInst);

		lMDinst.Init(lMDConfigMap,&lStrRunner);




		std::map<std::string,std::string> lTDConfigMap;
		boost::property_tree::ptree lTDpt;
		read_xml("TDConfig.xml",lTDpt);
		lTDConfigMap["BrokerID"] = lTDpt.get<std::string>("TDConfig.BrokerID");
		lTDConfigMap["UserID"] =  lTDpt.get<std::string>("TDConfig.UserID");
		lTDConfigMap["Password"]= lTDpt.get<std::string>("TDConfig.Password");
		lTDConfigMap["Front"] = lTDpt.get<std::string>("TDConfig.Front");
		lTDConfigMap["StoreDir"] = lTDpt.get<std::string>("TDConfig.StoreDir");

		lTdInst.Init(lTDConfigMap,&lStrRunner);

		while(true)
		{
			boost::this_thread::sleep(boost::posix_time::millisec(1000));
		}
	}
	catch (std::exception& e)
	{
		std::cout<<e.what();
	}


}