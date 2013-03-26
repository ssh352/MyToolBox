#include "CloseStrategy.h"
#include <sstream>
#include <boost\tokenizer.hpp>
#include <myForeach.h>


#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>

using boost::posix_time::time_duration ;


static std::string  g_Instument = "IF1304";
static double          WinLevel1 = 5;
static double         QuitLevel1 = 2;
static double          WinLevel2 = 10;
static double         QuitLevel2 = 3;
static double          WinLevel3 = 15;
static double         QuitLevel3 = 4;


CloseStrategy::CloseStrategy(void)
{
}


CloseStrategy::~CloseStrategy(void)
{
}

void CloseStrategy::OnMarketDepth( const std::string& aMarketDepth )
{
  
	std::stringstream lbuf(aMarketDepth);
	using boost::property_tree::ptree;
	ptree pt;
	read_xml(lbuf,pt);

	std::string lInstrumentID = pt.get<std::string>("market.ID");

	if(g_Instument !=lInstrumentID ) 
		return;

	double lLastPrice  = pt.get<double>("market.LastPx");

	time_duration  lthisTicktime = boost::posix_time::duration_from_string ( pt.get<std::string>("market.Second",""));
	lthisTicktime += boost::posix_time::millisec( pt.get<int>("market.Millsecond",0));


	if(m_isPlaceOrder)
		return ;

	
	
	if(CheckTrigger(lLastPrice))
	{
		using boost::property_tree::ptree;
		ptree pt;
		pt.put("head.type","PlaceOrder");
		pt.put("head.version",0.1f);
		pt.put("Order.ID" , g_Instument);
		pt.put("Order.BuyCode" , "Sell");
		pt.put("Order.OpenCode" , "CloseT");
		pt.put("Order.Price" , lLastPrice );
		pt.put("Order.Vol" , 1 );

		std::stringstream lStringStream;
		write_xml(lStringStream,pt);
		m_ActiveOrder = m_pTD->CreateOrder(lStringStream.str());
	}



}

void CloseStrategy::OnRtnOrder( const std::string& apOrder )
{
	//std::stringstream lbuf(apOrder);
	//std::string lorderID ;
	//lbuf >> lorderID;
	//int		OrderStates;

}

void CloseStrategy::OnRtnTrade( const std::string& apTrade )
{
	//todo check the Traded Order ID
	m_ExitHandle();
	//todo held ok

}

void CloseStrategy::Reload()
{
	m_isPlaceOrder = false;
	m_MarketCache.clear();
	m_CheckLevel =0;
}

bool CloseStrategy::CheckTrigger( double aLastPrice )
{
	switch (m_CheckLevel)
	{
	case	0:
		return CheckLevel0(aLastPrice);
	case	1:
		return CheckLevel1(aLastPrice);
	case	2:
		return CheckLevel2(aLastPrice);
	case	3:
		return CheckLevel3(aLastPrice);
	default:
		assert(false);
	}
}

bool CloseStrategy::CheckLevel0( double aPrice )
{
	double differ = aPrice - m_StartPrice;
	if(differ <4.999)
	{
		//nothing
	}
	else if(differ < 9.999)
	{
		m_CheckLevel =1;
		m_Level1EnterPrice = aPrice;
	}
	else if(differ < 14.999)
	{
		m_CheckLevel = 2;
		m_Level2EnterPrice = aPrice;
	}
	else
	{
		m_CheckLevel = 3;
		m_Level3EnterPrice = aPrice;
	}

	return false;
}

bool CloseStrategy::CheckLevel1( double aPrice )
{
	double differ = aPrice - m_Level1EnterPrice;
	if(differ < -2.001 )
	{
		return true;
	}
	else  if(differ < 4.999)
	{
		 
	}
	else if(differ < 9.999)
	{
		m_CheckLevel =2;
		m_Level2EnterPrice = aPrice;
	}
	else 
	{
		m_CheckLevel = 3;
		m_Level3EnterPrice = aPrice;
	}
	return false;
}

bool CloseStrategy::CheckLevel2( double aPrice )
{
	double differ = aPrice - m_Level2EnterPrice;
	if(differ < -4.001 )
	{
		return true;
	}
	else  if(differ < 4.999)
	{

	}
	else 
	{
		m_CheckLevel =3;
		m_Level3EnterPrice = aPrice;
	}

	return false;
}

bool CloseStrategy::CheckLevel3( double aPrice )
{
	double differ = aPrice - m_Level3EnterPrice;
	if(differ < -6.001 )
	{
		return true;
	}
	
	return false;
}
