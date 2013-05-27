#include "StrategyDemo514.h"
#include "IDriver_TD.h"
#include "IStrategySpi.h"
#include "ISignalModule.h"
#include "MarketCache.h"

#include <boost\format.hpp>
#include <boost\property_tree\ptree.hpp>
#include <boost\property_tree\xml_parser.hpp>
#include <windows.h>
namespace AT
{
	StrategyDemo514::StrategyDemo514(const  char* aConfigFile,AT::IDriver_TD * apTD  , AT::IStrategySpi* apStrSpi,const AT::IMarketCache* apMarketCache)
		:m_pMarketCache(apMarketCache)
	{
		boost::property_tree::ptree lConfig;
		read_xml(aConfigFile,lConfig);

		for( std::pair<std::string,boost::property_tree::ptree>  lSingleMoudleList : lConfig.get_child("SignalLoaderStr.Signals"))
		{
			std::string lDllName = lSingleMoudleList.second.get<std::string>("SignalDllName");
			std::string lDllConfig =  lSingleMoudleList.second.get<std::string>("SignalDllConfig");
			m_SingleList.push_back(std::make_pair(lDllName,lDllConfig));
		}
	}


	StrategyDemo514::~StrategyDemo514(void)
	{
	}

	void StrategyDemo514::Start()
	{


		for(auto lPair : m_SingleList)
		{
			std::string lDllName = lPair.first;
			std::string lDllConfig = lPair.second;
			HMODULE  lSinglehandle = LoadLibrary(lDllName.c_str());
			if( ! lSinglehandle)
			{
				std::cout<<boost::format("Can not load SingleMoudle DLL %s")%lDllName;
				break;
			}
			CreateSignalInstFun lpSignalCallInst =(CreateSignalInstFun) GetProcAddress(lSinglehandle,"CreateSignal");
			if (! lpSignalCallInst)
			{
				std::cout<<boost::format("Can not Get Single Create Inst Fun Address");
				break;
			}
			AT::ISignalModule* lpSignalInst = lpSignalCallInst(lDllConfig.c_str(),m_pMarketCache);
			if(!lpSignalInst)
			{
				std::cout<<boost::format("failed Create SignalModule inst with ConfigFile %s  ")%lDllConfig;
				break;
			}
			m_SingleModuleVec.push_back(lpSignalInst);
			m_LibHandleVec.push_back(lSinglehandle);
		}

		for(auto lSignalPtr:m_SingleModuleVec)
		{
			lSignalPtr->Start();
		}
	}

	void StrategyDemo514::OnMarketDepth(const MarketData& aMarketDepth )
	{
		for(auto lSignalPtr:m_SingleModuleVec)
		{
			lSignalPtr->OnMarketDepth(aMarketDepth);
		}

	}

	void StrategyDemo514::Stop()
	{
		for(auto lSignalPtr:m_SingleModuleVec)
		{
			lSignalPtr->Stop();
		}

		for(auto lHandle :m_LibHandleVec)
		{
			FreeLibrary(lHandle);
		}
	}


}
