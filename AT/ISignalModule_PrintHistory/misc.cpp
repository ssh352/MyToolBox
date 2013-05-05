#include "SignalPrintHistory.h"

DLL_API  AT::ISignalModule* CreateSignal(const char* aConfigFile, const AT::IMarketCache * apMarketCache)
{
	return  new SignalPrintHistory(aConfigFile,apMarketCache);
}
