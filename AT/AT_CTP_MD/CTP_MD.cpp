#include "CTP_MD.h"

#include "IMarketSpi.h"
#include "DepthReceiveV2.h"
#include <boost\property_tree\ptree.hpp>
#include <boost\property_tree\xml_parser.hpp>


namespace CTP
{


	CTP_MD::CTP_MD( const char* aConfigFile,AT::IMarketSpi* apSpi )
		:m_MarketSpi(apSpi)
		,m_ConfigFilePath(aConfigFile)
	{
	}

	CTP_MD::~CTP_MD(void)
	{
	}

	void CTP_MD::Start()
	{
		boost::property_tree::ptree lPt;
		read_xml(m_ConfigFilePath,lPt);
		
		std::stringstream lbuf;
		write_xml(lbuf,lPt);



		m_pDepth.reset(new DepthReceiveV2(lbuf.str(),
			std::bind(&CTP_MD::OnMarketDepth,this,std::placeholders::_1), 
			std::bind(&CTP_MD::OnMarketStatus,this,std::placeholders::_1,std::placeholders::_2)
			));

		m_pDepth->Start();
	}

	void CTP_MD::Stop()
	{
		m_MarketSpi->NotifyStateMD(AT::EMarketState::STOP,"User Stop");
		m_pDepth->Stop();
	}

	void CTP_MD::OnMarketStatus( CTP_Market_Status_Enum aStatus,std::string aErrorMsg )
	{
		switch (aStatus)
		{
		case CTP::CTP_Market_Status_Enum::E_CTP_MD_CONNECTING:
			m_MarketSpi->NotifyStateMD(AT::EMarketState::START,aErrorMsg.c_str());
			break;
		case CTP::CTP_Market_Status_Enum::E_CTP_MD_LOGIN:
			break;
		case CTP::CTP_Market_Status_Enum::E_CTP_MD_READY:
			m_MarketSpi->NotifyStateMD(AT::EMarketState::READY,aErrorMsg.c_str());
			break;
		case CTP::CTP_Market_Status_Enum::E_CTP_MD_MARKET_DELAY:
			m_MarketSpi->NotifyStateMD(AT::EMarketState::WARN_MarketDelay,aErrorMsg.c_str());
			break;
		case CTP::CTP_Market_Status_Enum::E_CTP_MD_MARKET_LONG_DELAY:
			m_MarketSpi->NotifyStateMD(AT::EMarketState::WARN_MarketDelay,aErrorMsg.c_str());
			break;
		default:
			break;
		}
	}

	void CTP_MD::OnMarketDepth( std::shared_ptr< AT::MarketData> apMarket )
	{
		m_MarketSpi->NotifyMarketDepth(*apMarket);
	}

}

