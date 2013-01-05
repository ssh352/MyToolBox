#include "PrintMDSpi.h"
#include <iostream>


void PrintMDSpi::NotifyState( int aErrorCode,const std::string& aErrorMsg )
{
	std::cerr<<"NotifySubModuleState Error Code="<<aErrorCode <<"\nError Msg="<<aErrorMsg<<std::endl;
}

void PrintMDSpi::NotifyExchange( const std::string& aExchange )
{
	std::cerr<<aExchange<<std::endl;
}

void PrintMDSpi::NotifyProduct( const std::string& aProduct )
{
	std::cerr<<aProduct<<std::endl;
}

void PrintMDSpi::NotifyInstrument( const std::string& aInstrument )
{
	std::cerr<<aInstrument<<std::endl;
}

void PrintMDSpi::NotifyMarketDepth( const std::string& aMarketDepth )
{
	std::cerr<<aMarketDepth<<std::endl;
}
