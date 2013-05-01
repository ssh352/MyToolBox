#include "CTP_MD.h"

DLL_API AT::IDriver_MD* CreateMD(const char* aConfig, AT::IMarketSpi* apMarketSpi)
{
	return new CTP::CTP_MD(aConfig,apMarketSpi);
}
