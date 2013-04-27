#include "CTP_MD_Replayer.h"
#include "IMarketSpi.h"
#include <leveldb/db.h>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>
#include <boost/format.hpp>
#include <boost/lexical_cast.hpp>
#include <string>
namespace CTP
{


	CTP_MD_Replayer::CTP_MD_Replayer(const char* aConfig,AT::IMarketSpi* apTradeSpi )
		:m_pMarketSpi(apTradeSpi)
	{
		boost::property_tree::ptree pt;
		read_xml(aConfig,pt);

		for( std::pair<std::string,boost::property_tree::ptree>  lDbList : pt.get_child("MarketReplayer.DBList"))
		{
			m_ReplayList .insert( boost::lexical_cast<std::string >(lDbList.second.data()));
		}
		//for(auto lVale:)
		//{
		//	for(boost::property_tree::ptree::iterator iter = lDbList.begin();iter!=lDbList.end();++iter)
		//	{
		//		iter
		//		/*m_ReplayList.insert(lVale.second.data());*/
		//	}
		//	
		//}


		for (auto lDBString: m_ReplayList)
		{
			leveldb::DB*& lpDB = m_MarketDBMap[lDBString];
			leveldb::Options options;
			options.create_if_missing = false;
			leveldb::Status lstatus = leveldb::DB::Open(options, lDBString, &lpDB);
			if(!lstatus.ok())
			{
				std::cerr<<boost::format("Open DB %s failed  ") % lDBString;
			}
		}

		for(auto lPair: m_MarketDBMap)
		{
			leveldb::DB* m_pDB = lPair.second;
			leveldb::Iterator* liter = m_pDB->NewIterator(leveldb::ReadOptions());
			for (liter->SeekToFirst(); liter->Valid(); liter->Next()) 
			{
				uint32_t lKeyTime = 0;
				assert(liter->key().size() == sizeof(uint32_t));
				memcpy(&lKeyTime,liter->key().data(),sizeof(uint32_t));

				AT::MarketData lValMarket;
				assert(liter->value().size() == sizeof(lValMarket));
				memcpy(&lValMarket,liter->value().data(),sizeof(lValMarket));

				m_MarketTickMapStored.insert(std::make_pair(lKeyTime,lValMarket));
			}
			delete liter;
		}

	}

	CTP_MD_Replayer::~CTP_MD_Replayer(void)
	{
	}

	void CTP_MD_Replayer::Start()
	{
		m_pWorker = new boost::asio::io_service::work(m_IOService);
		m_MockIOThread  = std::thread( boost::bind(&(boost::asio::io_service::run),&m_IOService));

		m_IOService.post( boost::bind(&CTP_MD_Replayer::DoPost,this,m_MarketTickMapStored.begin()));

	}

	void CTP_MD_Replayer::Stop()
	{
		delete m_pWorker;
		if(m_MockIOThread.joinable()) m_MockIOThread.join();

		m_pMarketSpi->NotifyStateMD(AT::EMarketState::STOP,"CTP_MD_Replayer Stop Post");
	}

	void CTP_MD_Replayer::DoPost(MarketStoredMapType::iterator aPostIndex )
	{
		if (aPostIndex == m_MarketTickMapStored.begin())
		{
			m_pMarketSpi->NotifyStateMD(AT::EMarketState::READY,"CTP_MD_Replayer Ready First Post Come");
		}
		int cout =0;

		while(aPostIndex != m_MarketTickMapStored.end())
		{
			m_pMarketSpi->NotifyMarketDepth(aPostIndex->second);
			aPostIndex++;
			cout++;
		}

		m_pMarketSpi->NotifyStateMD(AT::EMarketState::END_MARKETDAY,str(boost::format( "All Market Have Posted MessageCount %d") % cout ).c_str());

	}





}
