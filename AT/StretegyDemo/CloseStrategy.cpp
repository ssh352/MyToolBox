#include "CloseStrategy.h"
#include "IDriver_TD.h"
#include <sstream>
#include <boost\tokenizer.hpp>


#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>

using boost::posix_time::time_duration ;




CloseStrategy::CloseStrategy(AT::IDriver_TD* apTD)
	:m_pTD(apTD)
{
	m_CheckLevel = 0;
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

	std::string lTImeSec = pt.get<std::string>("market.Second");
	std::string lMillSec = pt.get<std::string>("market.Millsecond");
	std::string lKey = lTImeSec + '.'+ lMillSec;

	if(m_Instument !=lInstrumentID ) 
		return;

	double lLastPrice  = pt.get<double>("market.LastPx");


	if(m_isPlaceOrder)
		return ;

	
	
	if(CheckTrigger(lLastPrice))
	{
		using boost::property_tree::ptree;
		ptree pt;
		pt.put("head.type","PlaceOrder");
		pt.put("head.version",0.1f);
		pt.put("Order.ID" , m_Instument);
		if(!m_IsSell)
			pt.put("Order.BuyCode" , "Sell");
		else
			pt.put("Order.BuyCode" , "Buy");
		pt.put("Order.OpenCode" , "CloseT");
		pt.put("Order.Price" , lLastPrice );
		pt.put("Order.Vol" , 1 );
		pt.put("Order.Time",lKey);

		std::stringstream lStringStream;
		write_xml(lStringStream,pt);
		m_ActiveOrder = m_pTD->CreateOrder(lStringStream.str());
		m_isPlaceOrder = true;
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
	std::stringstream lbuf(apTrade);
	using boost::property_tree::ptree;
	ptree pt;
	read_xml(lbuf,pt);
	std::string lOrderID= pt.get<std::string>("Trade.ThostOrderID");
	if(lOrderID == m_ActiveOrder)
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

	double Profoit ;
	if(!m_IsSell)
	{
		Profoit = aPrice - m_StartPrice;
	}
	else
	{
		Profoit = m_StartPrice - aPrice ;
	}

	if(Profoit < m_Level0QuitPrice)
	{
		return true;
	}
	else if(Profoit < m_Level1EnterPrice)
	{

	}
	else if(Profoit < m_Level2EnterPrice)
	{
		m_CheckLevel = 1;
	}
	else if(Profoit < m_Level3EnterPrice)
	{
		m_CheckLevel = 2;
	}
	else
	{
		m_CheckLevel = 3;
	}
	return false;
}

bool CloseStrategy::CheckLevel1( double aPrice )
{
	double Profoit ;
	if(!m_IsSell)
	{
		Profoit = aPrice - m_StartPrice;
	}
	else
	{
		Profoit = m_StartPrice - aPrice ;
	}

	if(Profoit < m_Level1QuitPrice)
	{
		return true;
	}
	else if(Profoit < m_Level2EnterPrice)
	{

	}
	else if(Profoit < m_Level3EnterPrice)
	{
		m_CheckLevel = 2;
	}
	else 
	{
		m_CheckLevel = 3;
	}

	return false;

}

bool CloseStrategy::CheckLevel2( double aPrice )
{

	double Profoit ;
	if(!m_IsSell)
	{
		Profoit = aPrice - m_StartPrice;
	}
	else
	{
		Profoit = m_StartPrice - aPrice ;
	}

	if(Profoit < m_Level2QuitPrice)
	{
		return true;
	}
	else if(Profoit < m_Level3EnterPrice)
	{

	}
	else 
	{
		m_CheckLevel = 3;
	}

	return false;
	

}

bool CloseStrategy::CheckLevel3( double aPrice )
{
	double Profoit ;
	if(!m_IsSell)
	{
		Profoit = aPrice - m_StartPrice;
	}
	else
	{
		Profoit = m_StartPrice - aPrice ;
	}

	if(Profoit < m_Level3QuitPrice)
	{
		return true;
	}
	return false;
}

void CloseStrategy::UpdateParam( AT::IStrategy::EStrInputState aSource,const std::string& apStrParam)
{
	std::stringstream lbuf(apStrParam);
	using boost::property_tree::ptree;
	ptree pt;
	read_xml(lbuf,pt);

	m_Instument = pt.get<std::string>("StrConfig.close.Instrument");

	 m_Level0EnterPrice =  pt.get<double>("StrConfig.close.Level0.EnterPrice");
	 m_Level1EnterPrice = pt.get<double>("StrConfig.close.Level1.EnterPrice");
	 m_Level2EnterPrice =  pt.get<double>("StrConfig.close.Level2.EnterPrice");
	 m_Level3EnterPrice =  pt.get<double>("StrConfig.close.Level3.EnterPrice");

	 m_Level0QuitPrice =  pt.get<double>("StrConfig.close.Level0.QuitPrice");
	 m_Level1QuitPrice =  pt.get<double>("StrConfig.close.Level1.QuitPrice");
	 m_Level2QuitPrice =  pt.get<double>("StrConfig.close.Level2.QuitPrice");
	 m_Level3QuitPrice =  pt.get<double>("StrConfig.close.Level3.QuitPrice");

	 std::cerr<<"SetStrategyPram  CloseStrategy  ======================\n"
				<< m_Level0EnterPrice<<"  "<<m_Level0QuitPrice<<"\n"
				<< m_Level1EnterPrice<<"  "<<m_Level1QuitPrice<<"\n"
				<< m_Level2EnterPrice<<"  "<<m_Level2QuitPrice<<"\n"
				<< m_Level3EnterPrice<<"  "<<m_Level3QuitPrice<<"\n";
			
}

void CloseStrategy::SetStartPrice( double aStartPrice,bool isSell )
{
	m_StartPrice = aStartPrice;
	m_IsSell = isSell;
	std::cerr<<"\n\n\nStart Close iSSell "<<m_IsSell<<" StartPrice = "<<m_StartPrice<<"\n\n\n";
}
