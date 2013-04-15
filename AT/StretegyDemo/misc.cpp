#include "StrDemo1.h"

 DLL_API AT::IStrategy* CreateStr(const std::string aConfigFile, AT::IDriver_TD* apTradeSpi ,AT::IStrategySpi* apStrSpi)
 {
	return new StrDemo1(aConfigFile,apTradeSpi,apStrSpi);
 }
