#pragma once
#include "AT_Struct.h"
#include <vector>
namespace AT
{
class IIndexModule;
class IMarketCache;
struct ItemParam
{
	int Item;
	int Param;
};
class IndexContainer
{
public:
	IndexContainer(const char* aConfigFile, AT::IMarketCache* apMarketCache);
	~IndexContainer(void);
	void OnMarketDepth(const AT::MarketData& aMarketDepth) ;
	//这个函数主要用来查询一段时间内指定名字的Index 以指定值出现的次数
	int GetIndexCount(const std::string& aIndexName,int ExpectVal,AT_Time aStartTime,AT_Time aEndTime);
	//取得 指定名字的Index 最新一个值
	int GetIndex( const std::string& aIndexName);

	//取得 指定名字Index 最后一个非0值
	int GetLastNonZero(const std::string& aIndexName);
	void Start();
	void Stop();

	//初始化加载指标
	void InitLoadIndex(const char* aConfigFile);
private:
	std::vector<IIndexModule*>		m_SignalModuleVec;
	typedef std::map<AT_Time,int>	SignalResultMap;
	std::map<std::string,SignalResultMap> m_SignalResultMapGroupBySignalName;
	AT::IMarketCache*				m_pMarketCache;
};


}
