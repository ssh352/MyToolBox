#pragma once
#include "AT_Struct.h"
#include <vector>
namespace AT
{
class IIndexModule;
class IMarketCache;

class IndexContainer
{
public:
	IndexContainer(const char* aConfigFile,const AT::IMarketCache* apMarketCache);
	~IndexContainer(void);
	void OnMarketDepth(const AT::MarketData& aMarketDepth) ;
	//���������Ҫ������ѯһ��ʱ����ָ�����ֵ�Index ��ָ��ֵ���ֵĴ���
	int GetIndexCount(const std::string& aIndexName,int ExpectVal,AT_Time aStartTime,AT_Time aEndTime);
	//ȡ�� ָ�����ֵ�Index ����һ��ֵ
	int GetIndex( const std::string& aIndexName,AT_Time aTime);

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
	const AT::IMarketCache*				m_pMarketCache;
};


}
