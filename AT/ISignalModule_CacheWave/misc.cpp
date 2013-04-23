#include "SignalModule_CacheWave.h"

DLL_API  AT::ISignalModule* CreateSignal(const std::string aConfigFile, const AT::MarketCache * apMarketCache)
{
	return  new AT::SignalModule_CacheWave(aConfigFile);
}
