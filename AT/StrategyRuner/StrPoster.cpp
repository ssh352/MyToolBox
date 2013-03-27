#include "StrPoster.h"
#include <iostream>
#include "CTP_MD.h"
#include "CTP_TD.h"
StrPoster::StrPoster(AT::IDriver_TD* apTD)
	: m_TDReady(false)
	, m_MDReady(false)
{
	m_Inst.SetTDPoint(apTD);
	m_Inst.SetupChild();
}
void StrPoster::NotifyState( int aErrorCode,const std::string& aErrorMsg )
{
	std::cerr<<"NotifySubModuleState Error Code="<<aErrorCode <<"\nError Msg="<<aErrorMsg<<std::endl;

	m_MDReady = (CTP::DepthReceiver_RECEIVE_STATE == aErrorCode);
	TryStartInst();

}

void StrPoster::NotifyExchange( const std::string& aExchange )
{
	std::cerr<<aExchange<<std::endl;
}

void StrPoster::NotifyProduct( const std::string& aProduct )
{
	std::cerr<<aProduct<<std::endl;
}

void StrPoster::NotifyInstrument( const std::string& aInstrument )
{
	std::cerr<<aInstrument<<std::endl;
}

void StrPoster::NotifyMarketDepth( const std::string& aMarketDepth )
try
{
	//std::cerr<<aMarketDepth<<std::endl;
	m_Inst.OnMarketDepth(aMarketDepth);
}
catch (std::exception& ex)
{
	std::cerr<<ex.what();
};

void StrPoster::OnRtnState( int aErrorCode,const std::string& aErrorMsg )
try
{
	m_TDReady = (CTP::Ready == aErrorCode);
	TryStartInst();
}
catch (std::exception& ex)
{
	std::cerr<<ex.what();
};

void StrPoster::OnRtnOrder( const std::string& apOrder )
try
{
	m_Inst.OnRtnOrder(apOrder);
	std::cout<<apOrder<<std::endl;
}
catch (std::exception& ex)
{
	std::cerr<<ex.what();
};


void StrPoster::OnRtnTrade(const std::string& apTrade)
try
{
	m_Inst.OnRtnTrade(apTrade);
	std::cout<<apTrade<<std::endl;
}
catch (std::exception& ex)
{
	std::cerr<<ex.what();
};
void StrPoster::OnRtnPosition( const std::string& aPosition )
{
	std::cout<<aPosition<<std::endl;
}

void StrPoster::ReSetParam( const std::string& aConfig )
{
	m_Inst.SetStrategyPram(aConfig);
}

void StrPoster::TryStartInst()
{
	if(m_TDReady && m_MDReady)
	{
		std::cerr<<"\n================================Æô¶¯³É¹¦=================================\m";
		m_Inst.Start();
	}
}


