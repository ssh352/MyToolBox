#pragma once
#include "AT_Struct.h"
#include <vector>
namespace AT
{
class ISignalModule;

class IndexContainer
{
public:
	IndexContainer(std::vector<ISignalModule*>);
	~IndexContainer(void);
	void OnMarketDepth(const AT::MarketData& aMarketDepth) ;
	int GetIndexCount(const std::string& aIndexName,int ExpectVal,AT_Time aStartTime,AT_Time aEndTime);
	void Start();
	void Stop();

private:
	std::vector<ISignalModule*>		m_SignalModuleVec;
	typedef std::map<AT_Time,int>	SignalResultMap;
	std::map<std::string,SignalResultMap> m_SignalResultMapGroupBySignalName;

};


}
