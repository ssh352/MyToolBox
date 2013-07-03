#include "SignalModule_CacheWave.h"

DLL_API  AT::ISignalModule* CreateSignal(const char* aConfigFile, const AT::IMarketCache * apMarketCache)
{
	return NULL;
//	return  new AT::SignalModule_CacheWave(aConfigFile);
}
