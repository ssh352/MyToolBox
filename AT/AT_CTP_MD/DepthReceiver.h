#pragma once

#include <string>
#include <boost\shared_ptr.hpp>
#include "CTP_API.h"
#include "CTP_MD.h"


namespace CTP
{
	class DataCacheCTP;
	class CTP_MD;
	class DepthReceiver :public CThostFtdcMdSpi
	{
	public:
		DepthReceiver(const std::string aConfigStr);
		void SetStateReceive(CTP_MD* parent,boost::shared_ptr<DataCacheCTP> apDataCacheCTP);
		void Start();
		void SubscribeInstrument(const std::string& aInstrumentID );
		void UnSubscribeInstrument(const std::string& aInstrumentID);
	private:
		CTP_MD*	m_pCTP_MD;
		boost::shared_ptr<DataCacheCTP> m_pDataCache;
		CThostFtdcMdApi*  m_pMDAPI;
	};

}


