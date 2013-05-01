#include "PrintMDSpi.h"
#include <iostream>




void PrintMDSpi::NotifyMarketDepth( const AT::MarketData& aMarketDepth )
{
	std::cout<<aMarketDepth.ToString()<<'\n';
}

void PrintMDSpi::NotifyStateMD( AT::EMarketState aErrorCode,const char* aErrorMsg )
{
	std::cout<<"Status ==========================  "<< static_cast<int>(aErrorCode) << "Message "<< aErrorMsg<<'\n';
}
