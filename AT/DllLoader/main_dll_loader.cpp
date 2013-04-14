#include <map>
#include <Windows.h>
#include "IDriver_TD.h"
#include "../TD_CommandLineTest/PrintTDSpi.h"
#include <fstream>
#include <iostream>


std::string GetCmd()
{
	char lCmd[512] ;
	memset(lCmd,0,512);
	std::cin.getline(lCmd,500,'\n');
	std::string lRet(lCmd);
	size_t lpos =lRet.find_first_not_of(" ");
	lRet = lRet.substr(lpos);
	return lRet;
}

std::string  CreateNewOrder(AT::IDriver_TD* lTDInst)
{

	std::string lNewOrder = GetCmd();
	std::string lRet =  lTDInst->CreateOrder(lNewOrder);
	std::cerr<<"New order ID= "<<lRet <<std::endl;
	return lRet;
}

void DelOrder(AT::IDriver_TD* lTDInst)
{
	std::string lDelOrder = GetCmd();
	lTDInst->DeleteOrder(lDelOrder);
}


int main()
{
	HMODULE  lhandle = LoadLibrary("AT_CTP_TD.dll");
	CreatTDInstFun lpCallInst =(CreatTDInstFun) GetProcAddress(lhandle,"CreateDriverInsance");
	

	std::fstream configFile("config.ini");

	std::string lBorkerid ;
	std::string lUserID;
	std::string lPassWord;
	std::string  lFront;
	configFile>>lBorkerid>>lUserID>>lPassWord>>lFront;
	configFile.close();
	configFile.clear();

	std::map<std::string,std::string> lConfigMap;
	//lConfigMap["Index"] = "3";
	lConfigMap["BrokerID"] =lBorkerid ;
	lConfigMap["UserID"] = lUserID;
	lConfigMap["Password"]=lPassWord;
	lConfigMap["Front"] = lFront;

	//std::map<std::string,std::string> lConfigMap;
	//lConfigMap["Index"] = "1";
	//lConfigMap["BrokerID"] ="2030" ;
	//lConfigMap["UserID"] = "000000005510";
	//lConfigMap["Password"]= "228636";
	//lConfigMap["Front"] = "tcp://asp-sim2-front1.financial-trading-platform.com:26205";

	//std::map<std::string,std::string> lConfigMap2;
	//lConfigMap2["Index"] = "2";
	//lConfigMap2["BrokerID"] ="2030" ;
	//lConfigMap2["UserID"] = "000000005509";
	//lConfigMap2["Password"]= "221949";
	//lConfigMap2["Front"] = "tcp://asp-sim2-front1.financial-trading-platform.com:26205";

	PrintTDSpi lSpiInst;
	AT::IDriver_TD* lpDriver = lpCallInst(lConfigMap,&lSpiInst);
	//AT::IDriver_TD* lpDriver2 = lpCallInst(lConfigMap2,&lSpiInst);



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
			CreateNewOrder(lpDriver);
			break;
		case DeleteOrder_CMD:
			DelOrder(lpDriver);
			break;
		case ModifyOrder_CMD:
			break;
		case QueryPos_CMD:
			lpDriver->QueryPosition("");
			break;
		}
		::Sleep(1000);
	}

	FreeLibrary(lhandle);

}