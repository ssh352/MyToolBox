#pragma  once
#include <string>
#include "AT_API_HEAD.h"

namespace AT
{
	class MarketCache;
	//Store All The History Market
	class ISignalModule
	{
	public:
		virtual void Start() =0;
		virtual void Stop() = 0;
		virtual int OnMarketDepth(const std::string& aMarketPtr) = 0;
	};

}
typedef  AT::ISignalModule* (*CreateSignalInstFun) (const std::string aConfigFile, const AT::MarketCache * apMarketCache);
DLL_API  AT::ISignalModule* CreateSignal(const std::string aConfigFile, const AT::MarketCache * apMarketCache);
