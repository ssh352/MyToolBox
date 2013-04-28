#include "SingleStrDLLLoaderPoster.h"
#include <iostream>
#include <IStrategy.h>
#include <IMarketCache.h>
namespace AT
{
	SingleStrDLLLoaderPoster::SingleStrDLLLoaderPoster(std::atomic<bool>& aflag , const char* aConfigFile)
		:m_FinishedFalg(aflag)
	{
		//todo load MarketCache
	}


	SingleStrDLLLoaderPoster::~SingleStrDLLLoaderPoster(void)
	{
	}

	void SingleStrDLLLoaderPoster::NotifyStateMD( EMarketState aErrorCode,const char * aErrorMsg )
	{
		std::cout<<"MD "<<static_cast<int>( aErrorCode) << "  Message :"<<aErrorMsg <<'\n';
		if(aErrorCode == EMarketState::END_MARKETDAY)
		{
			m_FinishedFalg = true;
		}
	}

	void SingleStrDLLLoaderPoster::NotifyStateStr( EStrState ErrorCode,const char* aErrorMsg )
	{
			std::cout<<"str "<<static_cast<int>( ErrorCode) << "  Message :"<<aErrorMsg <<'\n';
	}

	void SingleStrDLLLoaderPoster::NotifyStateTD( ETradeState aErrorCode,const char* aErrorMsg )
	{
		std::cout<<"TD "<<static_cast<int>( aErrorCode) << "  Message :"<<aErrorMsg <<'\n';
	}


	void SingleStrDLLLoaderPoster::NotifyMarketDepth( const MarketData& aMarketDepth )
	{
		m_pStr->OnMarketDepth(aMarketDepth);
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

