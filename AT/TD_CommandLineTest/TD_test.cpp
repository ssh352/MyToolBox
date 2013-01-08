#include "CTP_TD.h"

#include <boost/thread.hpp>
#include <boost/date_time.hpp>
#include "PrintTDSpi.h"
#include <iostream>
#include <fstream>

std::string GetCmd()
{
	char lCmd[512] ;
	memset(lCmd,0,512);
	std::cin.getline(lCmd,500,'\n');
	std::string lRet(lCmd);
	return lRet;
}

std::string  CreateNewOrder(CTP::CTP_TD& lTDInst)
{

	std::string lNewOrder = GetCmd();
	std::string lRet =  lTDInst.CreateOrder(lNewOrder);
	std::cerr<<"New order ID= "<<lRet <<std::endl;
	return lRet;
}

void DelOrder(CTP::CTP_TD& lTDInst)
{
	std::string lDelOrder = GetCmd();
	lTDInst.DeleteOrder(lDelOrder);
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
	configFile.clear();
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
		case DeleteOrder_CMD:
		DelOrder(linst);
		break;
		}

		boost::this_thread::sleep(boost::posix_time::millisec(1000));
	}
}