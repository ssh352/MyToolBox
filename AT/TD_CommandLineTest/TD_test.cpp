#include "CTP_TD.h"

#include <boost/thread.hpp>
#include <boost/date_time.hpp>
#include "PrintTDSpi.h"
#include <iostream>
#include <fstream>

std::string  CreateNewOrder(CTP::CTP_TD& lTDInst)
{
	char lNewOrder[512] ;
	memset(lNewOrder,0,512);
	std::cin.getline(lNewOrder,500,'\n');
	
	std::string lRet =  lTDInst.CreateOrder(lNewOrder);
	std::cerr<<"New order ID= "<<lRet <<std::endl;
	return lRet;
}

int main()
{
	CTP::CTP_TD linst;
	std::map<std::string,std::string> lConfigMap;

	std::fstream configFile("config.ini");

	std::string lBorkerid ;
	std::string lUserID;
	std::string lPassWord;
	std::string  lFront;
	configFile>>lBorkerid>>lUserID>>lPassWord>>lFront;
	configFile.close();
	//lConfigMap["BrokerID"] ="2030" ;
	//lConfigMap["UserID"] = "000000005510";
	//lConfigMap["Password"]= "228636";
	//lConfigMap["Front"] = "tcp://asp-sim2-front1.financial-trading-platform.com:26205"

	//lConfigMap["BrokerID"] ="7030" ;
	//lConfigMap["UserID"] = "000516";
	//lConfigMap["Password"]= "000516";
	//lConfigMap["Front"] = "tcp://221.232.155.116:41205";

	lConfigMap["BrokerID"] =lBorkerid ;
	lConfigMap["UserID"] = lUserID;
	lConfigMap["Password"]=lPassWord;
	lConfigMap["Front"] = lFront;

	PrintTDSpi lSpiInst;
	linst.Init(lConfigMap,&lSpiInst);

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