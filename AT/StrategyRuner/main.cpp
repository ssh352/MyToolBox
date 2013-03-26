#include <fstream>
#include <string>
#include <boost/thread.hpp>
#include <boost/date_time.hpp>

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>
#include "CTP_MD.h"
#include "StrPoster.h"
#include "../MD_PrintTest/PrintMDSpi.h"
#include "myForeach.h"

int main()
{
	CTP::CTP_MD linst;

	std::map<std::string,std::string> lMDConfigMap;

	boost::property_tree::ptree lpt;
	read_xml("MDConfig.xml",lpt);

	lMDConfigMap["EnableStateReceiver"] =   lpt.get("MDConfig.EnableStateReceiver", "0");
	lMDConfigMap["EnableSubscribeList"] =  lpt.get("MDConfig.EnableSubscribeList", "1");

	lMDConfigMap["IsReplay"] =  lpt.get("MDConfig.IsReplay", "0");
	lMDConfigMap["Front"]  = lpt.get("MDConfig.Front",  "tcp://asp-sim2-front1.financial-trading-platform.com:26213");

	std::string  lSubList;
	MYFOREACH(v, lpt.get_child("MDConfig.SubList"))
	{
		lSubList += (v.second.data());
		lSubList += " ";

	}
	lMDConfigMap["SubscribeList"] = lSubList;

	//PrintMDSpi lPrintMD;
	StrPoster lPrintMD;
	linst.Init(lMDConfigMap,&lPrintMD);
	while(true)
	{
		boost::this_thread::sleep(boost::posix_time::millisec(1000));
	}
}