#include <fstream>
#include <string>
#include <boost/thread.hpp>
#include <boost/date_time.hpp>
#include "CTP_MD.h"
#include "StrPoster.h"
#include "../MD_PrintTest/PrintMDSpi.h"
int main()
{
	CTP::CTP_MD linst;

	//std::fstream lConfigFile("config.ini");
	//std::string lSubListStr;
	//std::string lbufl;
	//lConfigFile>>lbufl;
	//lSubListStr.append( lbufl);
	//while(lConfigFile>>lbufl)
	//{
	//	lSubListStr.append( lbufl);
	//}
	std::map<std::string,std::string> lMDConfigMap;

	std::fstream configFile("MDconfig.ini");

	std::string lBorkerid ;
	std::string lUserID;
	std::string lPassWord;
	std::string  lFront;
	configFile>>lBorkerid>>lUserID>>lPassWord>>lFront;
	configFile.close();
	configFile.clear();

	//lMDConfigMap["Front"] = "tcp://asp-sim2-front1.financial-trading-platform.com:26213";
	//lMDConfigMap["EnableStateReceiver"] = "0";
	//lMDConfigMap["EnableSubscribeList"] = "1";
	//lMDConfigMap["SubscribeList"] = "IF1303 IF1304 IF1305 IF1306 IF1307";
	//lMDConfigMap["IsReplay"] = "0";

	//lConfigMap["SubscribeList"] = lSubListStr;


	lMDConfigMap["EnableStateReceiver"] = "0";
	lMDConfigMap["EnableSubscribeList"] = "1";
	lMDConfigMap["SubscribeList"] = "IF1304";
	lMDConfigMap["IsReplay"] = "0";
	lMDConfigMap["Front"]  = "tcp://asp-sim2-front1.financial-trading-platform.com:26213";



	//PrintMDSpi lPrintMD;
	StrPoster lPrintMD;
	linst.Init(lMDConfigMap,&lPrintMD);
	while(true)
	{
		boost::this_thread::sleep(boost::posix_time::millisec(1000));
	}
}