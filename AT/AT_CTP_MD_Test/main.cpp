

#include "CTP_MD.h"
#include "PrintMDSpi.h"

#include <fstream>
#include <string>
#include <boost/thread.hpp>
#include <boost/date_time.hpp>

#include <csignal>

bool g_isRunning = true;
void singaleHandle(int parame)
{
	std::cerr<<"singaleHandle  "<<parame;
	g_isRunning =false;
}



int main()
{
	signal(SIGINT,singaleHandle);
	PrintMDSpi lPrintMD;
	CTP::CTP_MD linst("MDConfig.xml",&lPrintMD);

	linst.Start();

	while(g_isRunning)
	{
		boost::this_thread::sleep(boost::posix_time::millisec(1000));
	}
}