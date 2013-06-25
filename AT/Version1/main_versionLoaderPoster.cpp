#include "main_versionLoaderPoster.h"
#include <IStrategy.h>

namespace AT
{
	main_versionLoaderPoster::main_versionLoaderPoster(std::atomic<bool>&  aFinishedFalg)
		:m_FinishedFalg(aFinishedFalg)
	{
	}


	main_versionLoaderPoster::~main_versionLoaderPoster(void)
	{
	}

	void main_versionLoaderPoster::NotifyStateMD( EMarketState aErrorCode,const char * aErrorMsg )
	{
		std::cout<<"MD "<<static_cast<int>( aErrorCode) << "  Message :"<<aErrorMsg <<'\n';
		if(aErrorCode == EMarketState::END_MARKETDAY)
		{
			m_FinishedFalg = true;
		}
	}

	void main_versionLoaderPoster::NotifyStateStr( EStrState ErrorCode,const char* aErrorMsg )
	{
		std::cout<<"str "<<static_cast<int>( ErrorCode) << "  Message :"<<aErrorMsg <<'\n';
	}

	void main_versionLoaderPoster::NotifyStateTD( ETradeState aErrorCode,const char* aErrorMsg )
	{
		std::cout<<"TD "<<static_cast<int>( aErrorCode) << "  Message :"<<aErrorMsg <<'\n';
	}


	void main_versionLoaderPoster::NotifyMarketDepth( const MarketData& aMarketDepth )
	{
		m_pStrategy->OnMarketDepth(aMarketDepth);
	}

	void main_versionLoaderPoster:: OnRtnOrder(const OrderUpdate& apOrder)
	{
		m_pStrategy->OnRtnOrder(apOrder);
	}

	void main_versionLoaderPoster::OnRtnTrade(const TradeUpdate& apTrade)
	{
		m_pStrategy->OnRtnTrade(apTrade);
	}
}
