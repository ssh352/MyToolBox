#include "StrDemo1.h"
#include <iostream>
#include <string>

StrDemo1::StrDemo1()
{
	m_isRuning= false;
	ChangeToOpenState();
	SetupChild();
}

StrDemo1::~StrDemo1(void)
{
	
}

void StrDemo1::OnMarketDepth( const std::string& aMarketDepth )
{
	//std::cout<<"StrDemo1 Receive "<<aMarketDepth;
	if(m_isRuning)
		m_CurrentState->OnMarketDepth(aMarketDepth);

}

void StrDemo1::OnRtnOrder( const std::string& apOrder )
{
	if(m_isRuning)
		m_CurrentState->OnRtnOrder(apOrder);
		std::cout<<"StrDemo1 Receive "<<apOrder;
}

void StrDemo1::OnRtnTrade( const std::string& apTrade )
{
	if(m_isRuning)
		m_CurrentState->OnRtnTrade(apTrade);
		std::cout<<"StrDemo1 Receive "<<apTrade;
}

void StrDemo1::NotifyState( const std::string& aErrorCode )
{

}

void StrDemo1::SetStrategyPram(const std::string& aConfig )
{

}



void StrDemo1::OnRtnPos( const std::string& aPos )
{

}
