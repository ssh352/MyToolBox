
#include <Windows.h>
#include "IDriver_TD.h"
#include <string>
#include "PrintTDSpi.h"
#include <thread>
#include "ATLogger.h"
#include <csignal>
bool g_isRunning = true;
void singaleHandle(int parame)
{
	std::cerr<<"singaleHandle  "<<parame;
	g_isRunning =false;
}




	

int main(int argc,char** argv)
{
	signal(SIGINT,singaleHandle);
	std::string lTDDllName = "AT_CTP_TD.dll";
	std::string lTDConfigFile ="C:\\GitTrunk\\MyToolBox\\AT\\AT_CTP_TD\\TDConfig.xml";
	ATLOG(AT::L_INFO,"Start TDTest");
	HMODULE  lTDhandle = LoadLibrary(lTDDllName.c_str());
	if( ! lTDhandle)
	{
		ATLOG(AT::L_ERROR,"Can not load TD DLL");
		return 2;
	}
	CreatTDInstFun lpTDCallInst =(CreatTDInstFun) GetProcAddress(lTDhandle,"CreateTD");
	if (! lpTDCallInst)
	{
		ATLOG(AT::L_ERROR,"Can not Get TD Create Inst Fun Address");
		return 3;
	}

	PrintTDSpi lPrintInst;

	AT::IDriver_TD* lpTDInst = lpTDCallInst(lTDConfigFile.c_str(),&lPrintInst);
	if(!lpTDInst)
	{
		std::cout<<"failed Create TD inst with ConfigFile";
		return 4;
	}
	
	lpTDInst->Start();
	while (g_isRunning)
	{
		std::this_thread::yield();
	}
	lpTDInst->Stop();
	//TODO there should be problem about dll create and exe release it . 
	delete lpTDInst;
	FreeLibrary(lTDhandle);
	return 0;
}