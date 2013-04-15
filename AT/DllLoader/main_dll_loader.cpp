#include <map>
#include <Windows.h>
#include "IDriver_TD.h"
#include "IDriver_MD.h"
#include "IStrategy.h"
#include "../TD_CommandLineTest/PrintTDSpi.h"
#include <fstream>
#include <iostream>

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>

#include "SingleStrDLLLoaderPoster.h"
#include <boost\format.hpp>
#include <boost\thread.hpp>
#include <boost\date_time.hpp>
//<Config>
//	<TDDLL>
//	<DLLFileName>C:\\GitHub\\MyToolBox\\AT\\sln\\Release\\AT_CTP_MockTD.dll</DLLFileName>
//	<DLLConfig>null</DLLConfig>
//	</TDDLL>
//
//	<MDDLL>
//	<DLLFileName>C:\\GitHub\\MyToolBox\\AT\\sln\\Release\\AT_CTP_MD_DataReplayer.dll</DLLFileName>
//	<DLLConfig>C:\\GitHub\\MyToolBox\\AT\\AT_CTP_MD_DataReplayer\\MDDataReplayer.xml</DLLConfig>  
//	</MDDLL>
//
//	<STRDLL>
//	<DLLFileName>C:\\GitHub\\MyToolBox\\AT\\sln\\Release\\StretegyDemo.dll</DLLFileName>
//	<DLLConfig>C:\\GitHub\\MyToolBox\\AT\\StrategyRuner\\IStrParam.xml</DLLConfig>  
//	</STRDLL>
//	</Config>




int main(int argc ,char** argv)
{

	if(argc < 2)
	{
		std::cout<<"参数不够 第一个参数因为配置文件";
		return 1;
	}


	std::string lConfigFile = argv[1];

	AT::SingleStrDLLLoaderPoster lPosterInst;

	
	boost::property_tree::ptree lPtree;
	read_xml(lConfigFile,lPtree);

	
	std::string lTDDllName = lPtree.get<std::string>("Config.TDDLL.DLLFileName");
	std::string lTDConfigFile = lPtree.get<std::string>("Config.TDDLL.DLLConfig");
	HMODULE  lTDhandle = LoadLibrary(lTDDllName.c_str());
	if( ! lTDhandle)
	{
		std::cout<<boost::format("Can not load TD DLL %s")%lTDDllName;
		return 2;
	}
	CreatTDInstFun lpTDCallInst =(CreatTDInstFun) GetProcAddress(lTDhandle,"CreateTD");
	if (! lpTDCallInst)
	{
		std::cout<<boost::format("Can not Get TD Create Inst Fun Address");
		return 3;
	}
	AT::IDriver_TD* lpTDInst = lpTDCallInst(lTDConfigFile,&lPosterInst);
	if(!lpTDInst)
	{
		std::cout<<boost::format("Faile Create TD inst with ConfigFile %s  ")%lTDConfigFile;
		return 4;
	}


	std::string lMDDllName = lPtree.get<std::string>("Config.MDDLL.DLLFileName");
	std::string lMDConfigFile = lPtree.get<std::string>("Config.MDDLL.DLLConfig");
	HMODULE  lMDhandle = LoadLibrary(lMDDllName.c_str());
	if( ! lMDhandle)
	{
		std::cout<<boost::format("Can not load MD DLL %s")%lMDDllName;
		return 5;
	}
	CreatMDInstFun lpMDCallInst =(CreatMDInstFun) GetProcAddress(lMDhandle,"CreateMD");
	if (! lpMDCallInst)
	{
		std::cout<<boost::format("Can not Get MD Create Inst Fun Address");
		return 6;
	}
	AT::IDriver_MD* lpMDInst = lpMDCallInst(lMDConfigFile,&lPosterInst);
	if(!lpMDInst)
	{
		std::cout<<boost::format("Faile Create MD inst with ConfigFile %s  ")%lMDConfigFile;
		return 7;
	}


	std::string lStrDllName = lPtree.get<std::string>("Config.STRDLL.DLLFileName");
	std::string lStrConfigFile = lPtree.get<std::string>("Config.STRDLL.DLLConfig");
	HMODULE  lStrhandle = LoadLibrary(lStrDllName.c_str());
	if( ! lStrhandle)
	{
		std::cout<<boost::format("Can not load Str DLL %s")%lStrDllName;
		return 8;
	}
	CreateStrInstFun lpStrCallInst =(CreateStrInstFun) GetProcAddress(lStrhandle,"CreateStr");
	if (! lpStrCallInst)
	{
		std::cout<<boost::format("Can not Get Str Create Inst Fun Address");
		return 9;
	}

	AT::IStrategy* lpStrInst = lpStrCallInst(lStrConfigFile,lpTDInst,&lPosterInst);
	if(!lpStrInst)
	{
		std::cout<<boost::format("Faile Create Str inst with ConfigFile %s  ")%lStrConfigFile;
		return 10;
	}



	lPosterInst.Setup(lpStrInst);

	lpTDInst->Start();
	lpStrInst->Start();
	lpMDInst->Start();

	int lQuitSingle;
	while(std::cin>>lQuitSingle)
	{
		if(lQuitSingle = 100)
			break;
		boost::this_thread::sleep(boost::posix_time::seconds(1));
	}

	lpMDInst->Stop();
	lpStrInst->Stop();
	lpTDInst->Stop();
	
	

	FreeLibrary(lTDhandle);
	FreeLibrary(lMDhandle);
	FreeLibrary(lStrhandle);

	return 0;

}