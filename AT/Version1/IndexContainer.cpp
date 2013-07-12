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

	void IndexContainer::InitLoadIndex(const char* aConfigFile)
	{
		m_SignalModuleVec.clear();

		boost::property_tree::ptree lConfig;
		read_xml(aConfigFile,lConfig);
		for( std::pair<std::string,boost::property_tree::ptree>  lIndexItem : lConfig.get_child("IndexContainerConfig.Indexs"))
		{
			std::string lDllName = lIndexItem.second.get<std::string>("Dll");
			std::string lDllConfig =  lIndexItem.second.get<std::string>("ConfigFile");
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
			m_SignalModuleVec.push_back(lpSignalInst);
			//todo store for clean  ÓÅÏÈ¼¶µÍ
			//m_LibHandleVec.push_back(lSinglehandle);
		}
	}
	IndexContainer::~IndexContainer(void)
	{
	}

void IndexContainer::OnMarketDepth( const AT::MarketData& aMarketDepth )
{
	for (auto lpSignal:m_SignalModuleVec)
	{
		int lresult = lpSignal->OnMarketDepth(aMarketDepth);
		if(lresult != Ignore_Market_result)
		{
			m_SignalResultMapGroupBySignalName[lpSignal->GetIndexName()] [aMarketDepth.m_UpdateTime] = lresult;
		}
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

	int IndexContainer::GetIndex( const std::string& aIndexName ,AT_Time aTime)
	{
		SignalResultMap& lResultMap = m_SignalResultMapGroupBySignalName[aIndexName];
		if(lResultMap.size() > 0)
		{
			//ToDO there should right, because the time may did not have the Market so it did not have the result
			//but in Production it should not a problem
			//return lResultMap.rbegin()->second;
			return lResultMap[aTime];
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
	std::map<AT_Time,int> ResultMap;
	for(auto Index:m_SignalModuleVec)
	{		
		Index->Start();
		ResultMap = Index->GetHistoryResult();
		if(ResultMap.size() != 0)		
		{
			m_SignalResultMapGroupBySignalName[Index->GetIndexName()] = ResultMap;
		}
	}
}

	void IndexContainer::Stop()
	{
		for(auto lSignalPtr:m_SignalModuleVec)
		{
			lSignalPtr->Stop();
		}
	}
}