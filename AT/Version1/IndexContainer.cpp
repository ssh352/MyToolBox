#include "IndexContainer.h"
#include "ISignalModule.h"
#include "ATLogger.h"
#include <boost/format.hpp>
#include "boost/property_tree/ptree.hpp"
#include "boost/property_tree/xml_parser.hpp"

#include "windows.h"

namespace AT
{


	IndexContainer::IndexContainer(const char* aConfigFile,const AT::IMarketCache* apMarketCache)
		:m_pMarketCache(apMarketCache)
	{
		InitLoadIndex(aConfigFile);
	}


IndexContainer::~IndexContainer(void)
{
}

void IndexContainer::OnMarketDepth( const AT::MarketData& aMarketDepth )
{
	for (auto lpSignal:m_SignalModuleVec)
	{
		int lresult = lpSignal->OnMarketDepth(aMarketDepth);
		m_SignalResultMapGroupBySignalName[lpSignal->GetIndexName()] [aMarketDepth.m_UpdateTime] = lresult;
	}
}

int IndexContainer::GetIndexCount( const std::string& aIndexName,int ExpectVal,AT_Time aStartTime,AT_Time aEndTime )
{

	SignalResultMap& lResultMap = m_SignalResultMapGroupBySignalName[aIndexName];
	SignalResultMap::iterator lStart = lResultMap.find(aStartTime);
	SignalResultMap::iterator lEnd =lResultMap.find(aEndTime);
	int lret = 0;
	if(lStart != lResultMap.end() && lEnd != lResultMap.end())
	{
		for(SignalResultMap::iterator iter = lStart; iter != lEnd ; iter++)
		{
			if(iter->second == ExpectVal)
			{
				lret++;
			}
		}
	}
	return lret;
}

int IndexContainer::GetIndex( const std::string& aIndexName )
{
	SignalResultMap& lResultMap = m_SignalResultMapGroupBySignalName[aIndexName];
	if(lResultMap.size() > 1)
	{
		return lResultMap.rbegin()->second;
	}
	else
	{
		ATLOG(L_INFO,"GetIndex when no MarketFeed");
		return 0;
	}
}

int IndexContainer::GetLastNonZero( const std::string& aIndexName )
{
	SignalResultMap& lResultMap = m_SignalResultMapGroupBySignalName[aIndexName];
	for (SignalResultMap::reverse_iterator iter = lResultMap.rbegin(); iter != lResultMap.rend(); ++iter)
	{
		if(iter->second != 0)
			return iter->second;
	}
	ATLOG(L_INFO,"GetLastNonZero ,but NonZero, so Return 0");
	return 0;
}


void IndexContainer::Start()
{
	for(auto lSignalPtr:m_SignalModuleVec)
	{
		lSignalPtr->Start();
	}
}

void IndexContainer::Stop()
{
	for(auto lSignalPtr:m_SignalModuleVec)
	{
		lSignalPtr->Stop();
	}
}
void IndexContainer::InitLoadIndex(const char* aConfigFile)
{
	m_SignalModuleVec.clear();

	boost::property_tree::ptree lConfig;
	read_xml(aConfigFile,lConfig);
	for( std::pair<std::string,boost::property_tree::ptree>  lSingleMoudleList : lConfig.get_child("IndexContainerConfig.Indexs"))
	{
		std::string lDllName = lSingleMoudleList.second.get<std::string>("Dll");
		std::string lDllConfig =  lSingleMoudleList.second.get<std::string>("ConfigFile");
		std::string lDllIndexName = lSingleMoudleList.second.get<std::string>("IndexName");
		HMODULE  lSinglehandle = LoadLibrary(lDllName.c_str());
		if( ! lSinglehandle)
		{
			ATLOG(AT::LogLevel::L_ERROR,str(boost::format("Can not load SingleMoudle DLL %s")%lDllName));
			break;
		}
		CreateSignalInstFun lpSignalCallInst =(CreateSignalInstFun) GetProcAddress(lSinglehandle,"CreateSignal");
		if (! lpSignalCallInst)
		{
			ATLOG(AT::LogLevel::L_ERROR,"Can not Get Single Create Inst Fun Address");
			break;
		}
		AT::IIndexModule* lpSignalInst = lpSignalCallInst(lDllConfig.c_str(),m_pMarketCache);
		if(!lpSignalInst)
		{
			ATLOG(AT::LogLevel::L_ERROR,str(boost::format("failed Create IndexMoudle inst with ConfigFile %s  ")%lDllConfig));
			break;
		}
		lpSignalInst->SetIndexName(lDllIndexName);
		m_SignalModuleVec.push_back(lpSignalInst);
		//todo store for clean  ”≈œ»º∂µÕ
		//m_LibHandleVec.push_back(lSinglehandle);
	}
}




}