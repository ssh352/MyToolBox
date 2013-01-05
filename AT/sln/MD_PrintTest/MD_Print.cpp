#include "CTP_MD.h"

#include <boost/thread.hpp>
#include <boost/date_time.hpp>
#include "PrintMDSpi.h"
int main()
{
	CTP::CTP_MD linst;
	std::map<std::string,std::string> lConfigMap;
	lConfigMap["EnableStateReceiver"] = "0";
	lConfigMap["EnableSubscribeList"] = "1";
	lConfigMap["SubscribeList"] = "IF1301 IF1302 IF1303";

	PrintMDSpi lPrintMD;
	linst.Init(lConfigMap,&lPrintMD);
	while(true)
	{
		boost::this_thread::sleep(boost::posix_time::millisec(1000));
	}
}