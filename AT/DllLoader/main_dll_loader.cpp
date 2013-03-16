#include <map>
#include <Windows.h>
#include "IDriver_TD.h"
#include "../TD_CommandLineTest/PrintTDSpi.h"


typedef AT::IDriver_TD* (*CreatInstFun)();

int main()
{
	HMODULE  lhandle = LoadLibrary("AT_CTP_TD.dll");
	CreatInstFun lpCallInst;
	lpCallInst =(CreatInstFun) GetProcAddress(lhandle,"CreateDriverInsance");
	AT::IDriver_TD* lpDriver = lpCallInst();

	std::map<std::string,std::string> lConfigMap;
	lConfigMap["BrokerID"] ="2030" ;
	lConfigMap["UserID"] = "000000005510";
	lConfigMap["Password"]= "228636";
	lConfigMap["Front"] = "tcp://asp-sim2-front1.financial-trading-platform.com:26205";


	PrintTDSpi lSpiInst;
	lpDriver->Init(lConfigMap,&lSpiInst);
	while(true)
	{
		::Sleep(1000);
	}

}