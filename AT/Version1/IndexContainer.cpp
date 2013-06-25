#include "IndexContainer.h"
#include "ISignalModule.h"
namespace AT
{


IndexContainer::IndexContainer(std::vector<ISignalModule*> apMoudleList)
: m_SignalModuleVec(apMoudleList)
{
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
	for(SignalResultMap::iterator iter = lStart; iter != lEnd ; iter++)
	{
		if(iter->second == ExpectVal)
		{
			lret++;
		}
	}
	return lret;
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

}