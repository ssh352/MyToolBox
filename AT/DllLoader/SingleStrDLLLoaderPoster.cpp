#include "SingleStrDLLLoaderPoster.h"
#include <iostream>
#include <IStrategy.h>
namespace AT
{
	SingleStrDLLLoaderPoster::SingleStrDLLLoaderPoster(std::atomic<bool>& aflag )
		:m_FinishedFalg(aflag)
	{
	}


	SingleStrDLLLoaderPoster::~SingleStrDLLLoaderPoster(void)
	{
	}

	void SingleStrDLLLoaderPoster::NotifyStateMD( EMarketState aErrorCode,const char * aErrorMsg )
	{
		std::cout<<static_cast<int>( aErrorCode) << "  Message :"<<aErrorMsg <<'\n';
		if(aErrorCode == EMarketState::END_MARKETDAY)
		{
			m_FinishedFalg = true;
		}
		
	}



	void SingleStrDLLLoaderPoster::NotifyMarketDepth( const MarketData& aMarketDepth )
	{
		m_pStr->OnMarketDepth(aMarketDepth);
	}

	void SingleStrDLLLoaderPoster::NotifyStateTD( ETradeState aErrorCode,const char* aErrorMsg )
	{
		std::cout<<static_cast<int>( aErrorCode) << "  Message :"<<aErrorMsg <<'\n';
	}

	void SingleStrDLLLoaderPoster:: OnRtnOrder(const OrderUpdate& apOrder)
	{
		m_pStr->OnRtnOrder(apOrder);
	}

	void SingleStrDLLLoaderPoster::OnRtnTrade(const TradeUpdate& apTrade)
	{
		m_pStr->OnRtnTrade(apTrade);
	}

}

