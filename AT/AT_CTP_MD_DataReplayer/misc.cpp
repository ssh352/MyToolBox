#include "CTP_MD_Replayer.h"

DLL_API AT::IDriver_MD* CreateMD(const char*  aMarketDBPath, AT::IMarketSpi* apMarketSpi)
{
	return new CTP::CTP_MD_Replayer(aMarketDBPath,apMarketSpi);
}