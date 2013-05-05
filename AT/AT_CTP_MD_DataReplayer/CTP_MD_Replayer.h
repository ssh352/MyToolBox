#pragma once
#include "IDriver_MD.h"

#include <memory>
#include <map>
#include <string>
#include <set>
#include <boost\asio.hpp>
#include <thread>

namespace AT
{
	class IMarketCache;
}


namespace CTP
{
	class CTP_MD_Replayer :public AT::IDriver_MD
	{
	public:
		CTP_MD_Replayer(const char* aReplayDBDir,AT::IMarketSpi* apMarketSpi);
		virtual ~CTP_MD_Replayer(void);
		virtual void UpdateParam(const AT::Param& aParam) override {} ;
		virtual void Start() override;
		virtual void Stop() override;
		virtual AT::IMarketCache* GetMarketCache() override;

	private:
		 void DoPost();

		std::multimap<uint64_t,AT::MarketData >  m_MarketTickMapStored;
		 AT::IMarketSpi* m_pMarketSpi;
		 std::thread m_MockIOThread;
		 boost::asio::io_service m_IOService;
		 boost::asio::io_service::work* m_pWorker;
		 std::unique_ptr<AT::IMarketCache> m_pMarketCache;
	};
}


