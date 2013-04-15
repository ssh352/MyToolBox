#include "SingleStrDLLLoaderPoster.h"
#include <iostream>
#include <IStrategy.h>
namespace AT
{
	SingleStrDLLLoaderPoster::SingleStrDLLLoaderPoster(void)
	{
	}


	SingleStrDLLLoaderPoster::~SingleStrDLLLoaderPoster(void)
	{
	}

	void SingleStrDLLLoaderPoster::NotifyStateMD( EMarketState aErrorCode,const std::string& aErrorMsg )
	{
		std::cout<<static_cast<int>( aErrorCode) << "  Message :"<<aErrorMsg <<'\n';
		
	}

	void SingleStrDLLLoaderPoster::NotifyExchange( const std::string& aExchange )
	{

	}

	void SingleStrDLLLoaderPoster::NotifyProduct( const std::string& aProduct )
	{

	}

	void SingleStrDLLLoaderPoster::NotifyInstrument( const std::string& aInstrument )
	{

	}

	void SingleStrDLLLoaderPoster::NotifyMarketDepth( const std::string& aMarketDepth )
	{
		m_pStr->OnMarketDepth(aMarketDepth);
		//std::cout<< aMarketDepth;
	}

	void SingleStrDLLLoaderPoster::NotifyStateTD( ETradeState aErrorCode,const std::string& aErrorMsg )
	{

	}

	void SingleStrDLLLoaderPoster::OnRtnOrder( const std::string& apOrder )
	{
		m_pStr->OnRtnOrder(apOrder);
	}

	void SingleStrDLLLoaderPoster::OnRtnTrade( const std::string& apTrade )
	{
		m_pStr->OnRtnTrade(apTrade);
	}

	void SingleStrDLLLoaderPoster::OnRtnPosition( const std::string& aPosition )
	{
		m_pStr->OnRtnPos(aPosition);
	}




}

