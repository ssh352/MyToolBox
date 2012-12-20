#include "CTP_MD.h"

#include "IMarketSpi.h"

#include "StateReceiver.h"
#include "DepthReceiver.h"

namespace CTP
{
	CTP_MD::CTP_MD(void)
	{
	}


	CTP_MD::~CTP_MD(void)
	{
	}

	void CTP_MD::Init( const std::string& aConfigStr,AT::IMarketSpi* apMarketSpi )
	{

		m_MarketSpi = apMarketSpi;
		m_pStateReceiver.reset(new StateReceiver(aConfigStr));
		m_pStateReceiver->SetStateReceive(this);
		m_pStateReceiver->Start();

		//m_pDepthReceiver.reset(new DepthReceiver);
		//m_pDepthReceiver->SetDepthReceive(this);
		//m_pDepthRecviver->Start();
	}

	void CTP_MD::NotifyExchange( const std::string& aExchange )
	{
		m_MarketSpi->NotifyExchange(aExchange);
	}

	void CTP_MD::NotifyProduct( const std::string& aProduct )
	{
		m_MarketSpi->NotifyProduct(aProduct);
	}

	void CTP_MD::NotifyInstrument( const std::string& aInstrument )
	{
		m_MarketSpi->NotifyInstrument(aInstrument);
	}

	void CTP_MD::NotifyMarketDepth( const std::string& aMarketDepth )
	{
		m_MarketSpi->NotifyMarketDepth(aMarketDepth);
	}

	void CTP_MD::NotifySubModuleState( int aErrorCode,const std::string& aErrorMsg )
	{

	}

}

