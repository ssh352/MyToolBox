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
	//���������Ҫ������ѯһ��ʱ����ָ�����ֵ�Index ��ָ��ֵ���ֵĴ���
	int GetIndexCount(const std::string& aIndexName,int ExpectVal,AT_Time aStartTime,AT_Time aEndTime);
	//ȡ�� ָ�����ֵ�Index ����һ��ֵ
	int GetIndex( const std::string& aIndexName);

	//ȡ�� ָ������Index ���һ����0ֵ
	int GetLastNonZero(const std::string& aIndexName);
	void Start();
	void Stop();

	//��ʼ������ָ��
	void InitLoadIndex(const char* aConfigFile);
private:
	std::vector<IIndexModule*>		m_SignalModuleVec;
	typedef std::map<AT_Time,int>	SignalResultMap;
	std::map<std::string,SignalResultMap> m_SignalResultMapGroupBySignalName;
	AT::IMarketCache*				m_pMarketCache;
};


}
