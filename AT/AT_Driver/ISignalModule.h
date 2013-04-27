#pragma  once
#include "AT_API_HEAD.h"
#include "AT_Struct.h"

namespace AT
{
	class IMarketCache;
	//Store All The History Market
	class ISignalModule
	{
	public:
		virtual void Start() =0;
		virtual void Stop() = 0;
		virtual int OnMarketDepth(const MarketData& aMarketPtr) = 0;
	};

}
typedef  AT::ISignalModule* (*CreateSignalInstFun) (const char* aConfigFile, const AT::IMarketCache * apMarketCache);
DLL_API  AT::ISignalModule* CreateSignal(const char* aConfigFile, const AT::IMarketCache * apMarketCache);
