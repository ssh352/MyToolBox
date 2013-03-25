#include "OpenStrategy.h"

#include <boost\tokenizer.hpp>
#include <myForeach.h>

using boost::posix_time::time_duration ;



time_duration  Parser( const std::string& aMarket , double& aRefPrice )
{
	boost::char_separator<char> sep(" ");
	boost::tokenizer<boost::char_separator<char>> lTok(aMarket,sep);
	std::vector<std::string> lStrList(lTok.begin(),lTok.end());

	aRefPrice  = std::stod( lStrList[3]);
	time_duration  lSecond = boost::posix_time::duration_from_string (lStrList[1]);
	lSecond += boost::posix_time::millisec(std::stoi(lStrList[2]));
	return lSecond;
}

OpenStrategy::OpenStrategy(void)
{
}


OpenStrategy::~OpenStrategy(void)
{
}

void OpenStrategy::OnMarketDepth( const std::string& aMarketDepth )
{
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
			m_MarketCache.clear();
			m_Handler(lLastPrice);
			break;
		}

	}
}
