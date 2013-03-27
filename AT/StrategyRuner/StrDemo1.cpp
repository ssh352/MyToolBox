#include "StrDemo1.h"
#include <iostream>
#include <string>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>
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
	 m_openState.SetStrategyPram(aConfig);
	 m_clostState.SetStrategyPram(aConfig);
}



void StrDemo1::OnRtnPos( const std::string& aPos )
{

}

void StrDemo1::SetupChild()
{
	m_openState.SetTDPoint(m_pTD);
	m_openState.SendExitHandle(std::bind(&StrDemo1::ChangeToClostState,this,std::placeholders::_1,std::placeholders::_2));
	m_clostState.Reload();

	m_clostState.SetTDPoint(m_pTD);
	m_clostState.SendExitHandle(std::bind(&StrDemo1::ChangeToOpenState,this));
	m_clostState.Reload();


	boost::property_tree::ptree lConfigPT;
	read_xml("IStrParam.xml",lConfigPT);
	std::stringstream lbuf;
	write_xml(lbuf,lConfigPT);
	std::string lret = lbuf.str();
	SetStrategyPram(lret);
}

