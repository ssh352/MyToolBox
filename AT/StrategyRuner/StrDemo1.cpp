#include "StrDemo1.h"
#include <iostream>
#include <string>



StrDemo1::~StrDemo1(void)
{
}

void StrDemo1::OnMarketDepth( const std::string& aMarketDepth )
{
	//std::cout<<"StrDemo1 Receive "<<aMarketDepth;
	m_CurrentState->OnMarketDepth(aMarketDepth);

}

void StrDemo1::OnRtnOrder( const std::string& apOrder )
{
		m_CurrentState->OnMarketDepth(apOrder);
		std::cout<<"StrDemo1 Receive "<<apOrder;
}

void StrDemo1::OnRtnTrade( const std::string& apTrade )
{
		m_CurrentState->OnMarketDepth(apTrade);
		std::cout<<"StrDemo1 Receive "<<apTrade;
}

void StrDemo1::NotifyState( int aErrorCode )
{

}

void StrDemo1::SetStrategyPram(const std::string& aConfig )
{

}

StrDemo1::StrDemo1()
{
	SetupChild();
}

void StrDemo1::OnRtnPos( const std::string& aPos )
{

}
