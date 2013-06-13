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
	//todo
	return 0;
}


}