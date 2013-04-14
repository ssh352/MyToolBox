#pragma once
#include "IDriver_MD.h"

#include <leveldb/db.h>
#include <map>
#include <string>

namespace CTP
{
	class CTP_MD_Replayer :public AT::IDriver_MD
	{
	public:
		CTP_MD_Replayer();
		virtual ~CTP_MD_Replayer(void);

		virtual void Init(const std::map<std::string,std::string>& aConfigMap,AT::IMarketSpi* apTradeSpi);

		void Start();


	private:
		 std::map<std::string,leveldb::DB* > m_MarketDBMap;
		 std::map<std::string,std::string> m_MarketTickMapStored;
		 AT::IMarketSpi* m_pTradeSpi;

	};
}


