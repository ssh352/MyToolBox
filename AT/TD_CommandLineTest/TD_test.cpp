#include "CTP_TD.h"

#include <boost/thread.hpp>
#include <boost/date_time.hpp>
#include "PrintTDSpi.h"
int main()
{
	CTP::CTP_TD linst;
	std::map<std::string,std::string> lConfigMap;


	 lConfigMap["BrokerID"] ="2030" ;
	 lConfigMap["UserID"] = "000000005510";
	 lConfigMap["Password"]= "228636";
	 //strcpy_s(lLoginReq.BrokerID,11,"2030");
	 //strcpy_s(lLoginReq.UserID,15,"000000005510");
	 //strcpy_s(lLoginReq.Password,41,"228636");
	PrintTDSpi lSpiInst;
	linst.Init(lConfigMap,&lSpiInst);
	while(true)
	{
		boost::this_thread::sleep(boost::posix_time::millisec(1000));
	}
}