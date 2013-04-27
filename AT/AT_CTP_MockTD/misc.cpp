#include "CTP_TD_Mock.h"
DLL_API AT::IDriver_TD* CreateTD(const  char*  aConfig, AT::ITradeSpi* apTradeSpi)
{
	return new CTP::CTP_TD_Mock(aConfig,apTradeSpi);
}