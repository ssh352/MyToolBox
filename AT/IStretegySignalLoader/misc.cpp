#include "StretegySignalLoader.h"

DLL_API AT::IStrategy*   CreateStr(const char*  aConfigFile, AT::IDriver_TD* apTradeSpi ,AT::IStrategySpi* aStrSpi, const  AT::IMarketCache* apMarketCache)
{
	return new AT::StretegySignalLoader(aConfigFile,apTradeSpi,aStrSpi,apMarketCache);
}
