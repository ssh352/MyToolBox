#include "StrDemo1.h"
#include <iostream>
#include <string>



StrDemo1::~StrDemo1(void)
{
}

void StrDemo1::OnMarketDepth( const std::string& aMarketDepth )
{
	//std::cout<<"StrDemo1 Receive "<<aMarketDepth;
	m_Handler->OnMarketDepth(aMarketDepth);

}

void StrDemo1::OnRtnOrder( const std::string& apOrder )
{
		std::cout<<"StrDemo1 Receive "<<apOrder;
}

void StrDemo1::OnRtnTrade( const std::string& apTrade )
{
		std::cout<<"StrDemo1 Receive "<<apTrade;
}

void StrDemo1::NotifyState( int aErrorCode )
{

}

void StrDemo1::SetStrategyPram( AT::StrategyPram* apStrParam )
{

}

StrDemo1::StrDemo1()
{
	m_Handler = &m_openSate;
}
