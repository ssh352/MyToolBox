#include "StretegySignalLoader.h"

DLL_API AT::IStrategy* CreateStr(const std::string aConfigFile, AT::IDriver_TD* apTradeSpi ,AT::IStrategySpi* apStrSpi)
{
	return new AT::StretegySignalLoader(aConfigFile,apTradeSpi,apStrSpi);
}
