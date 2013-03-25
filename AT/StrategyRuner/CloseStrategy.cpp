#include "CloseStrategy.h"
#include <sstream>
#include <boost\tokenizer.hpp>
#include <myForeach.h>

using boost::posix_time::time_duration ;


static std::string  g_Instument = "IF1304";
static double          WinLevel1 = 5;
static double         QuitLevel1 = 2;
static double          WinLevel2 = 10;
static double         QuitLevel2 = 3;
static double          WinLevel3 = 15;
static double         QuitLevel3 = 4;


extern time_duration  Parser( const std::string& aMarket , double& aRefPrice );



CloseStrategy::CloseStrategy(void)
{
}


CloseStrategy::~CloseStrategy(void)
{
}

void CloseStrategy::OnMarketDepth( const std::string& aMarketDepth )
{
	//todo fliter others
	aMarketDepth;

	double lLastPrice ;
	
	time_duration lthisTicktime = Parser(aMarketDepth,  lLastPrice);
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
			
			std::string lOrderStr ("IF1304 ");
			lOrderStr += "buy ";
			lOrderStr += "open ";
			lOrderStr += "1 ";
			lOrderStr += std::to_string(lLastPrice);
			m_ActiveOrder = m_pTD->CreateOrder(lOrderStr);
			std::cout<<"place order ";
			break;
		}

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
}
