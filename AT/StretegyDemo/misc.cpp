#include "StrDemo1.h"


 DLL_API AT::IStrategy* CreateStr( AT::IDriver_TD* apTradeSpi)
 {
	return new StrDemo1(apTradeSpi);
 }
