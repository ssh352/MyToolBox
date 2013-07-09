#include "CTP_MD.h"

#include "IMarketSpi.h"
#include "DepthReceiveV2.h"
#include "MarketCache.h"
#include <boost\property_tree\ptree.hpp>
#include <boost\property_tree\xml_parser.hpp>
#include <boost\filesystem.hpp>

namespace CTP
{


	CTP_MD::CTP_MD( const char* aConfigFile,AT::IMarketSpi* apSpi )
		:m_MarketSpi(apSpi)
		,m_ConfigFilePath(aConfigFile)
	{
		boost::property_tree::ptree lPt;
		read_xml(m_ConfigFilePath,lPt);

		std::stringstream lbuf;
		write_xml(lbuf,lPt);

		std::string lCachePos = lPt.get<std::string>("MDConfig.MarketCache");
		boost::filesystem::path lDir(lCachePos);

		std::string lDataString = boost::gregorian::to_iso_string(AT::AT_Local_Time().date());

		if (!boost::filesystem::exists(lDir))
		{
			create_directory(lDir);
		}
		lDir /= lDataString;
		m_pMarketCache.reset(new AT::MarketCache(lDir.string().c_str()));

		m_pDepth.reset(new DepthReceiveV2(lbuf.str(),
			std::bind(&CTP_MD::OnMarketDepth,this,std::placeholders::_1), 
			std::bind(&CTP_MD::OnMarketStatus,this,std::placeholders::_1,std::placeholders::_2)
			));
	}

	CTP_MD::~CTP_MD(void)
	{
	}

	void CTP_MD::Start()
	{
		

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
		m_pMarketCache->FeedMarketDepth(apMarket);
		m_MarketSpi->NotifyMarketDepth(*apMarket);
	}

	AT::IMarketCache* CTP_MD::GetMarketCache()
	{
		return m_pMarketCache.get();
	}

}

