#include "CTP_TD_Mock.h"
DLL_API AT::IDriver_TD* CreateTD(const std::string& aConfig, AT::ITradeSpi* apTradeSpi)
{
	return new CTP::CTP_TD_Mock(aConfig,apTradeSpi);
}