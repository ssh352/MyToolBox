#pragma once
#include "AT_Struct.h"
#include <vector>
namespace AT
{
class ISignalModule;

struct ItemParam
{
	int Item;
	int Param;
};
class IndexContainer
{
public:
	IndexContainer(const char* aConfigFile);
	~IndexContainer(void);
	void OnMarketDepth(const AT::MarketData& aMarketDepth) ;
	int GetIndexCount(const std::string& aIndexName,int ExpectVal,AT_Time aStartTime,AT_Time aEndTime);
	int GetIndex( const std::string& aIndexName,int iIndex,AT_Time aStartTime,AT_Time aEndTime );
	void Start();
	void Stop();

	//初始化加载指标
	void InitLoadIndex(const char* aConfigFile);
private:
	std::vector<ISignalModule*>		m_SignalModuleVec;
	typedef std::map<AT_Time,int>	SignalResultMap;
	std::map<std::string,SignalResultMap> m_SignalResultMapGroupBySignalName;

};


}
