#include "Index_CacheWave.h"

DLL_API  AT::IIndexModule* CreateSignal(const char* aConfigFile, const AT::IMarketCache * apMarketCache)
{
	//return NULL;
	return  new AT::Index_CacheWave(aConfigFile,  apMarketCache);
}
