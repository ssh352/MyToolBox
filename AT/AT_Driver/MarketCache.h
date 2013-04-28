#pragma  once
#include "IMarketCache.h"
#include <memory>

namespace leveldb
{
	class DB;
}
namespace AT
{
	class MarketCache :public IMarketCache
	{
		
	public:

		MarketCache(const char* AconfigFile);
		~MarketCache(void);
		virtual const_MapWarpper_ptr  GetMarketMapByName(const char* aInstrument) const  override ;

		virtual void FeedMarketDepth(const MarketData& aMarketDepth) override;

	private:


		//�����е��� 
		//MarketMap��ʵ�ʱ����map ��m_InstrumentMarket ��������Ȩ
		//m_AllMarketMap ����MarketMapWarpper�������map���а�װ�����ǲ���������Ȩ
		//m_InstrumentDBMap ���ǽ��б�����������ݿ�

		std::map<std::string, const_MapWarpper_ptr > m_AllMarketMap;//for all Instrument
		std::map<std::string,std::shared_ptr< MarketMap> > m_InstrumentMarket; // for update 
		std::map<std::string ,leveldb::DB*> m_InstrumentDBMap;

	};
}

