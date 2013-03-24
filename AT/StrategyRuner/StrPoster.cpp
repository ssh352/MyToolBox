#include "StrPoster.h"
#include <iostream>


void StrPoster::NotifyState( int aErrorCode,const std::string& aErrorMsg )
{
	std::cerr<<"NotifySubModuleState Error Code="<<aErrorCode <<"\nError Msg="<<aErrorMsg<<std::endl;
}

void StrPoster::NotifyExchange( const std::string& aExchange )
{
	std::cerr<<aExchange<<std::endl;
}

void StrPoster::NotifyProduct( const std::string& aProduct )
{
	std::cerr<<aProduct<<std::endl;
}

void StrPoster::NotifyInstrument( const std::string& aInstrument )
{
	std::cerr<<aInstrument<<std::endl;
}

void StrPoster::NotifyMarketDepth( const std::string& aMarketDepth )
{
	m_Inst.OnMarketDepth(aMarketDepth);
}
