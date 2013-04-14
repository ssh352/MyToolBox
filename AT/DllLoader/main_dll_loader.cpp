#include <map>
#include <Windows.h>
#include "IDriver_TD.h"
#include "../TD_CommandLineTest/PrintTDSpi.h"
#include <fstream>
#include <iostream>



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


	PrintTDSpi lSpiInst;
	AT::IDriver_TD* lpDriver = lpCallInst(lConfigMap,&lSpiInst);
	//AT::IDriver_TD* lpDriver2 = lpCallInst(lConfigMap2,&lSpiInst);


	FreeLibrary(lhandle);

}