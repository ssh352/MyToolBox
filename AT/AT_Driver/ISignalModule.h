#pragma  once
#include <string>
#include "AT_API_HEAD.h"

namespace AT
{
	class MarketCache;
	class ISignalModule
	{
		virtual int OnMarket(const std::string& aMarketPtr) = 0;
	};

}

typedef  AT::ISignalModule* (*CreateSignalInstFun) (const std::string aConfigFile, const AT::MarketCache * apMarketCache);
DLL_API  AT::ISignalModule* CreateStr(const std::string aConfigFile, const AT::MarketCache * apMarketCache);