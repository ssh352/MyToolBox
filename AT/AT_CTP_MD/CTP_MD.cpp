#include "CTP_MD.h"

#include "IMarketSpi.h"

#include "StateReceiver.h"
#include "DepthReceive.h"
#include <iostream>
#include "DataCacheCTP.h"
#include <boost/foreach.hpp>
#include <boost/tokenizer.hpp>
namespace CTP
{
	CTP_MD::CTP_MD(void)
	{
	}


	CTP_MD::~CTP_MD(void)
	{
	}

	void CTP_MD::Init(const std::map<std::string,std::string>&  aConfigMap,AT::IMarketSpi* apMarketSpi)
	{
		InitConfig(aConfigMap);

		m_pDataCache.reset(new DataCacheCTP);
		m_MarketSpi = apMarketSpi;
		m_pDepthReceiver.reset(new DepthReceiver(aConfigMap));
		m_pDepthReceiver->SetDepthReceive(this,m_pDataCache);

		if(m_EnableState)
		{
			m_pStateReceiver.reset(new StateReceiver(aConfigMap));
			m_pStateReceiver->SetStateReceive(this,m_pDataCache);
			m_pStateReceiver->Start();
		}
		else
		{
			m_pDepthReceiver->Start();
		}

	
	}

	void CTP_MD::NotifyExchange( const std::string& aExchange )
	{
		std::cerr<<aExchange<<std::endl;
		//m_MarketSpi->NotifyExchange(aExchange);
	}

	void CTP_MD::NotifyProduct( const std::string& aProduct )
	{
		std::cerr<<aProduct<<std::endl;
		//m_MarketSpi->NotifyProduct(aProduct);
	}

	void CTP_MD::NotifyInstrument( const std::string& aInstrument )
	{
		std::cerr<<aInstrument<<std::endl;
		//m_MarketSpi->NotifyInstrument(aInstrument);
	}

	void CTP_MD::NotifyMarketDepth( const std::string& aMarketDepth )
	{
		std::cerr<<aMarketDepth<<std::endl;
		//m_MarketSpi->NotifyMarketDepth(aMarketDepth);
	}

	void CTP_MD::NotifySubModuleState( int aErrorCode,const std::string& aErrorMsg )
	{
		std::cerr<<"NotifySubModuleState Error Code="<<aErrorCode <<"\nError Msg="<<aErrorMsg<<std::endl;
		switch(aErrorCode)
		{
		case StateReceiver_RETRIEVE_DYNAMIC_STATE:
			m_pDepthReceiver->Start();
			break;
		case DepthReceiver_RECEIVE_STATE:
			SubSucribeAll();
			break;
		default:
			break;
		}
	}

	void CTP_MD::SubSucribeAll()
	{
		if(m_EnableSubscribeList)
		{
			BOOST_FOREACH(std::string lInstrumentName,m_SubscribeList)
			{
				m_pDepthReceiver->SubscribeInstrument(lInstrumentName);
			}
		}
		else
		{
			std::vector<std::string> lList = m_pDataCache->GetInstrumentListAll();
			BOOST_FOREACH(std::string lInstrumentName,lList)
			{
				m_pDepthReceiver->SubscribeInstrument(lInstrumentName);
			}
		}

	}

	void CTP_MD::InitConfig( const std::map<std::string, std::string>& aConfigMap )
	{
		m_ConfigMap = aConfigMap;
		m_EnableState = m_ConfigMap["EnableStateReceiver"] == "1";
		m_EnableSubscribeList = m_ConfigMap["EnableSubscribeList"] == "1";
		if(m_EnableSubscribeList)
		{
			std::string& lListString=m_ConfigMap["SubscribeList"];
			boost::tokenizer<> tok(lListString);
			BOOST_FOREACH(std::string lVar,tok)
			{
				m_SubscribeList.insert(lVar);
			}
		}
	}

}

