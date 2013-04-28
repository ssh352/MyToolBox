#pragma  once
#include "IMarketCache.h"
#include <memory>
namespace AT
{
	class MarketCache :public IMarketCache
	{
		
	public:
		MarketCache(const char* AconfigFile);
		~MarketCache(void);
		virtual const_MapWarpper_ptr  GetMarketMapByName(const char* aInstrument) const  override ;

	private:

		std::map<std::string, const_MapWarpper_ptr > m_MarketMap;

	};
}

