#include "OpenStrategy.h"
#include <sstream>
#include <boost\tokenizer.hpp>
#include <myForeach.h>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>
using boost::posix_time::time_duration ;


static std::string  g_Instument = "IF1304";
static int    g_timeDiv = 5;



OpenStrategy::OpenStrategy(void)
{
}


OpenStrategy::~OpenStrategy(void)
{
}

void OpenStrategy::OnMarketDepth( const std::string& aMarketDepth )
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
	lthisTicktime += boost::posix_time::millisec( pt.get<int>("market.Millsecond",""));
	
	for(auto each = m_MarketCache.begin();each!= m_MarketCache.end();)
	{
		if((lthisTicktime - each->first ) > boost::posix_time::minutes(5))
		{
			each = m_MarketCache.erase (each);
		}	
		else
		{
			each++;
		}
	}


	m_MarketCache[lthisTicktime] = lLastPrice;

	if(m_isPlaceOrder)
		return ;


	double low =100000;
	time_duration lowtime;
	MYFOREACH(each , m_MarketCache)
	{
		if(each.second < low ) 
		{
			low =each.second;
			lowtime =each.first;
		}

		if(each.second -low > 5)
		{
			std::cout<< "At time"<< each.first << "Price "<< low << "and Time "<<lthisTicktime << "Price "<< lLastPrice;	

		using boost::property_tree::ptree;
		ptree pt;
		pt.put("head.type","PlaceOrder");
		pt.put("head.version",0.1f);
		pt.put("Order.ID" , g_Instument);
		pt.put("Order.BuyCode" , "Buy");
		pt.put("Order.OpenCode" , "Open");
		pt.put("Order.Price" , lLastPrice );
		pt.put("Order.Vol" , 1 );
		
		std::stringstream lStringStream;
		write_xml(lStringStream,pt);
		m_ActiveOrder = m_pTD->CreateOrder(lStringStream.str());

		
			break;
		}

	}
}

void OpenStrategy::OnRtnOrder( const std::string& apOrder )
{
	//std::stringstream lbuf(apOrder);
	//std::string lorderID ;
	//lbuf >> lorderID;
	//int		OrderStates;

}

void OpenStrategy::OnRtnTrade( const std::string& apTrade )
{
	//todo check the Traded Order ID
	m_ExitHandle(m_placePrice);
	//todo held ok

}

void OpenStrategy::Reload()
{
	m_isPlaceOrder = false;
	m_MarketCache.clear();
}
