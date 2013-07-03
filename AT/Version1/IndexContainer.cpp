#include "IndexContainer.h"
#include "ISignalModule.h"
#include <boost/format.hpp>
#include "boost/property_tree/ptree.hpp"
#include "boost/property_tree/xml_parser.hpp"
#include "../ISignalModule_CacheWave/SignalModule_CacheWave.h"

namespace AT
{


IndexContainer::IndexContainer(const char* aConfigFile/*std::vector<ISignalModule*> apMoudleList*/)
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
int IndexContainer::GetIndex( const std::string& aIndexName,int iIndex,AT_Time aStartTime,AT_Time aEndTime )
{
	SignalResultMap& lResultMap = m_SignalResultMapGroupBySignalName[aIndexName];
	SignalResultMap::iterator lStart = lResultMap.find(aStartTime);
	SignalResultMap::iterator lEnd =lResultMap.find(aEndTime);
	int lret = 0;
	if(lStart == lResultMap.end())
	{
		lStart = lResultMap.begin();
	}
	if(lStart != lResultMap.end() && lEnd != lResultMap.end())
	{
		for(SignalResultMap::iterator iter = lStart; iter != lEnd ; iter++)
		{
			lret++;
			if(lret == iIndex )
			{
				return iter->second;
			}
		}
	}
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
	boost::property_tree::ptree lIndexPt;
	read_xml(aConfigFile,lIndexPt);
	int iItemID ;
	std::string strItemName;
	int iItemParam;
	std::map<std::string,ItemParam> mapItem;
	for (auto lSignal:lIndexPt.get_child("SignalConfig.Signals"))
	{
		for (auto lIndex:lSignal.second.get_child("HKY"))
		{
			iItemID = lIndex.second.get<int>("ItemID");
			strItemName = lIndex.second.get<std::string>("ItemName");
			iItemParam = lIndex.second.get<int>("ParamID");
			ItemParam itemPara = {iItemID,iItemParam};
			if(mapItem.size() > 0)
			{
				if(mapItem.find(strItemName) != mapItem.end())
				{
					mapItem[strItemName] = itemPara;
				}
			}
			else
			{
				mapItem[strItemName] = itemPara;
			}
			
		}
	}
	//加载所有的Item
	for(std::map<std::string,ItemParam>::const_iterator iter = mapItem.begin();iter != mapItem.end();++iter)
	{
		switch (iter->second.Item)
		{
		case 6:  //HK006
			{
				SignalModule_CacheWave* pModule = new SignalModule_CacheWave(iter->second.Param);
				pModule->SetIndexName(iter->first);
				m_SignalModuleVec.push_back(pModule);
			}
			break;
		case 1://HK001
			{

			}
			break;
		case 2://HK002
			{

			}
			break;;
		case 3://HK003
			{

			}
			break;
		default:
			break;
		}
	}
}
}