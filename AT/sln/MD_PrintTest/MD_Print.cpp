#include "CTP_MD.h"

#include <boost/thread.hpp>
#include <boost/date_time.hpp>
int main()
{
	CTP::CTP_MD linst;
	std::map<std::string,std::string> lConfigMap;
	linst.Init(lConfigMap,NULL);
	while(true)
	{
		boost::this_thread::sleep(boost::posix_time::millisec(1000));
	}
}