#pragma once
#include "IDriver_MD.h"

#include <leveldb/db.h>
#include <map>
#include <string>
#include <set>
#include <boost\asio.hpp>
#include <thread>

namespace CTP
{
	class CTP_MD_Replayer :public AT::IDriver_MD
	{
	public:
		CTP_MD_Replayer(const std::string& aConfig,AT::IMarketSpi* apTradeSpi);
		virtual ~CTP_MD_Replayer(void);
		virtual void UpdateParam(const std::string& apParam){};
		virtual void Start();
		virtual void Stop();


	private:

		 void DoPost( std::multimap<std::string,std::string>::iterator aPostIndex);
		 std::map<std::string,leveldb::DB* > m_MarketDBMap;
		 std::multimap<std::string,std::string> m_MarketTickMapStored;
		 AT::IMarketSpi* m_pMarketSpi;

		 std::set<std::string> m_ReplayList;

		 std::thread m_MockIOThread;
		 boost::asio::io_service m_IOService;
		 boost::asio::io_service::work* m_pWorker;
	};
}


