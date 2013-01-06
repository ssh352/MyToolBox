#include "CTP_TD.h"

#include <boost/thread.hpp>
#include <boost/date_time.hpp>
#include "PrintTDSpi.h"
#include <iostream>

std::string  CreateNewOrder(CTP::CTP_TD& lTDInst)
{
	char lNewOrder[512] ;
	memset(lNewOrder,0,512);
	std::cin.getline(lNewOrder,500,'\n');
	
	std::string lRet =  lTDInst.CreateOrder(lNewOrder);
	std::cerr<<"建立新单 单号= "<<lRet <<std::endl;
	return lRet;
}

int main()
{
	CTP::CTP_TD linst;
	std::map<std::string,std::string> lConfigMap;


	 lConfigMap["BrokerID"] ="2030" ;
	 lConfigMap["UserID"] = "000000005510";
	 lConfigMap["Password"]= "228636";
	PrintTDSpi lSpiInst;
	linst.Init(lConfigMap,&lSpiInst);

	//boost::this_thread::sleep(boost::posix_time::seconds(5));
	//linst.CreateOrder("IF1301 buy open 1 2550");

	const int  CreateNewOrder_CMD= 1;
	const int 	DeleteOrder_CMD = 2;
	const int  ModifyOrder_CMD = 3;
	const int QueryPos_CMD = 4;
	int lCmdMode;
	while(std::cin>>lCmdMode)
	{
		switch(lCmdMode)
		{
		case CreateNewOrder_CMD:
		CreateNewOrder(linst);
		break;
		}

		boost::this_thread::sleep(boost::posix_time::millisec(1000));
	}
}