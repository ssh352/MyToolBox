#include "OpenStrategy.h"
#include <sstream>
#include <boost\tokenizer.hpp>
#include <myForeach.h>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>

using boost::posix_time::time_duration ;





OpenStrategy::OpenStrategy(AT::IDriver_TD* apTD)
	:IStrategy(apTD)
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

	if(m_Instument !=lInstrumentID ) 
		return;

	double lLastPrice  = pt.get<double>("market.LastPx");

	time_duration  lthisTicktime = boost::posix_time::duration_from_string ( pt.get<std::string>("market.Second",""));
	lthisTicktime += boost::posix_time::millisec( pt.get<int>("market.Millsecond",0));
	
	for(auto each = m_MarketCache.begin();each!= m_MarketCache.end();)
	{
		if((lthisTicktime - each->first ) > boost::posix_time::seconds(m_TriggerTimeSeconds))
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

	double high  =0;
	time_duration hightime;

	MYFOREACH(each , m_MarketCache)
	{
		if(each.second < low ) 
		{
			low =each.second;
			lowtime =each.first;
		}

		if(each.second > high ) 
		{
			high =each.second;
			hightime =each.first;
		}

		if(each.second -low > m_TriigerPrice)
		{
			std::cout<< "Buy"<< lLastPrice ;	

		using boost::property_tree::ptree;
		ptree pt;
		pt.put("head.type","PlaceOrder");
		pt.put("head.version",0.1f);
		pt.put("Order.ID" , m_Instument);
		pt.put("Order.BuyCode" , "Buy");
		pt.put("Order.OpenCode" , "Open");
		pt.put("Order.Price" , lLastPrice );
		pt.put("Order.Vol" , 1 );
		m_placePrice = lLastPrice;
		std::stringstream lStringStream;
		write_xml(lStringStream,pt);
		m_ActiveOrder = m_pTD->CreateOrder(lStringStream.str());
		m_isPlaceOrder = true;
		m_IsSell = false;
			break;
		}

		if(each.second -high < -1*m_TriigerPrice)
		{
			std::cout<< "Sell "<< lLastPrice;	

			using boost::property_tree::ptree;
			ptree pt;
			pt.put("head.type","PlaceOrder");
			pt.put("head.version",0.1f);
			pt.put("Order.ID" , m_Instument);
			pt.put("Order.BuyCode" , "Sell");
			pt.put("Order.OpenCode" , "Open");
			pt.put("Order.Price" , lLastPrice );
			pt.put("Order.Vol" , 1 );
			m_placePrice = lLastPrice;
			std::stringstream lStringStream;
			write_xml(lStringStream,pt);
			m_ActiveOrder = m_pTD->CreateOrder(lStringStream.str());
			m_isPlaceOrder = true;
			
			m_IsSell = true;
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
	
	std::stringstream lbuf(apTrade);
	using boost::property_tree::ptree;
	ptree pt;
	read_xml(lbuf,pt);
	std::string lOrderID= pt.get<std::string>("Trade.ThostOrderID");
	if(lOrderID == m_ActiveOrder)
	{
		std::cerr<<"\n\n\n quit open state with price "<<m_placePrice << "\n"<< m_IsSell<<"\n\n\n";
		m_ExitHandle(m_placePrice,m_IsSell);
	}

}

void OpenStrategy::Reload()
{
	m_isPlaceOrder = false;
	m_MarketCache.clear();
}

void OpenStrategy::SetStrategyPram( const std::string& apStrParam )
{
	std::stringstream lbuf(apStrParam);
	using boost::property_tree::ptree;
	ptree pt;
	read_xml(lbuf,pt);

	m_Instument = pt.get<std::string>("StrConfig.open.Instrument");
	m_TriggerTimeSeconds = pt.get<int>("StrConfig.open.TimeTrigger");
	 m_TriigerPrice = pt.get<double>("StrConfig.open.TriggerPrice");

	 std::cerr<<" \n\n\nOpenStrategy SetStrategyPram"<<
		m_Instument << "\n"<<
			m_TriggerTimeSeconds << "\n"<<
				m_TriigerPrice << "\n\n\n";

}
