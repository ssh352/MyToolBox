#include "StrPoster.h"
#include <iostream>

StrPoster::StrPoster()
{
	//m_Inst.SetupChild();
}
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
	std::cerr<<aMarketDepth<<std::endl;
	m_Inst.OnMarketDepth(aMarketDepth);
}

void StrPoster::OnRtnState( int aErrorCode,const std::string& aErrorMsg )
{
	std::cout<<aErrorMsg<<std::endl;
}

void StrPoster::OnRtnOrder( const std::string& apOrder )
{
	m_Inst.OnRtnOrder(apOrder);
	std::cout<<apOrder<<std::endl;
}


void StrPoster::OnRtnTrade(const std::string& apTrade)
{
	m_Inst.OnRtnTrade(apTrade);
	std::cout<<apTrade<<std::endl;
}

void StrPoster::OnRtnPosition( const std::string& aPosition )
{
	std::cout<<aPosition<<std::endl;
}


