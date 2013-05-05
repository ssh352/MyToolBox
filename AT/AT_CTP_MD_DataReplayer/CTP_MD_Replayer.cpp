#include "CTP_MD_Replayer.h"
#include "IMarketSpi.h"
#include "MarketCache.h"
#include <leveldb/db.h>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>
#include <boost/format.hpp>
#include <string>
#include "MarketMapWarpper.h"
namespace CTP
{


	CTP_MD_Replayer::CTP_MD_Replayer(const char* aConfig,AT::IMarketSpi* apMarketSpi )
		:m_pMarketSpi(apMarketSpi)
	{

		m_pMarketCache.reset(new AT::MarketCache(aConfig));

		AT::MarketCacheAllInstruments lAllCached = m_pMarketCache->GetAllInstruments();


		for(int i = 0; i!= lAllCached.m_CacheSize;++i)
		{
			for (auto lMarket : (*lAllCached.m_CacheList[i])  )
			{
				uint64_t lkey = lMarket.m_UpdateTime.time_of_day().total_milliseconds();
				m_MarketTickMapStored.insert(std::make_pair(lkey,lMarket) );
			}
		}

	}

	CTP_MD_Replayer::~CTP_MD_Replayer(void)
	{
	}

	void CTP_MD_Replayer::Start()
	{
		m_pWorker = new boost::asio::io_service::work(m_IOService);
		m_MockIOThread  = std::thread( boost::bind(&(boost::asio::io_service::run),&m_IOService));
		m_IOService.post( boost::bind(&CTP_MD_Replayer::DoPost,this));

	}

	void CTP_MD_Replayer::Stop()
	{
		delete m_pWorker;
		if(m_MockIOThread.joinable()) m_MockIOThread.join();

		m_pMarketSpi->NotifyStateMD(AT::EMarketState::STOP,"CTP_MD_Replayer Stop Post");
	}

	void CTP_MD_Replayer::DoPost()
	{
		uint64_t cout = 0;
		for(auto lpair:m_MarketTickMapStored)
		{
			cout++;
			m_pMarketSpi->NotifyMarketDepth(lpair.second);
		}
		m_pMarketSpi->NotifyStateMD(AT::EMarketState::END_MARKETDAY,str(boost::format( "All Market Have Posted MessageCount %lu") % cout ).c_str());

	}

	AT::IMarketCache* CTP_MD_Replayer::GetMarketCache()
	{
		return m_pMarketCache.get();
	}





}
