

#include "CTP_TD.h"


DLL_API AT::IDriver_TD* CreateTD(const char* aConfigFile, AT::ITradeSpi* apTradeSpi)
{
	return new CTP::CTP_TD(aConfigFile,apTradeSpi);
}
