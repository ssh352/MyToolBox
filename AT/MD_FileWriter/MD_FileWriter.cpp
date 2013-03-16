#include <boost/thread.hpp>
#include <boost/date_time.hpp>
#include "FileWriterMDSpi.h"
#include "CTP_MD.h"
#include <csignal>

#include <boost/atomic/atomic.hpp>

boost::atomic<bool> g_runing ;

void singaleHandle(int parame)
{
	std::cerr<<"singaleHandle  "<<parame;
	g_runing = false;
}
int main()
{
	g_runing = true;
	signal(SIGINT,singaleHandle);
	CTP::CTP_MD linst;
	std::map<std::string,std::string> lConfigMap;

	//std::fstream lConfigFile("config.ini");
	//std::string lSubListStr;
	//std::string lbufl;
	//lConfigFile>>lbufl;
	//lSubListStr.append( lbufl);
	//while(lConfigFile>>lbufl)
	//{
	//	lSubListStr.append( lbufl);
	//}


	lConfigMap["EnableStateReceiver"] = "0";
	lConfigMap["EnableSubscribeList"] = "1";
	lConfigMap["SubscribeList"] = "IF1304 IF1305 IF1306 IF1307";
	//lConfigMap["SubscribeList"] = lSubListStr;


	FileWriterMDSpi lPrintMD("test.log");
	linst.Init(lConfigMap,&lPrintMD);
	while(g_runing)
	{
		boost::this_thread::sleep(boost::posix_time::millisec(1000));
	}
	return 0;
}