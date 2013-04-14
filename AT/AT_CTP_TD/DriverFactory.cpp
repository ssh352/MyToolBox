
#ifdef AT_LIB
#include "IDriver_TD.h"
#include "CTP_TD.h"


extern "C"  __declspec(dllexport) AT::IDriver_TD* CreateDriverInsance(const std::map<std::string,std::string>& aConfig,  AT::ITradeSpi* apTradeSpi)
{
	AT::IDriver_TD* lpDriverInstance = new CTP::CTP_TD();
	lpDriverInstance->Init(aConfig, apTradeSpi);
	return lpDriverInstance;
}
#endif