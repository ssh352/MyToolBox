#include "PrintTDSpi.h"
#include <iostream>


void PrintTDSpi::OnRtnState( int aErrorCode,const std::string& aErrorMsg )
{
	std::cerr<<"NotifySubModuleState Error Code="<<aErrorCode <<"\nError Msg="<<aErrorMsg<<std::endl;
}

void PrintTDSpi::OnRtnOrder( const std::string& apOrder )
{
	std::cerr<<"OnRtnOrder ="<<apOrder <<std::endl;
}

void PrintTDSpi::OnRtnTrade( const std::string& apTrade )
{
	std::cerr<<"OnRtnTrade ="<<apTrade <<std::endl;
}

void PrintTDSpi::OnRtnPosition( const std::string& aPosition )
{
	std::cerr<<"OnRtnPosition ="<<aPosition <<std::endl;
}
