#pragma once
#include "IDriver_MD.h"
#include <boost\shared_ptr.hpp>
#include <map>
#include <string>
#include <set>
#include <AT_Struct.h>

namespace AT
{
	class IMarketSpi;
}
namespace CTP
{
	enum class CTP_Market_Status_Enum;
	class DepthReceiveV2;

	class CTP_MD : public AT::IDriver_MD
	{
	public:
		CTP_MD(const char* aConfigFile,AT::IMarketSpi* apSpi);
		virtual ~CTP_MD(void);

	public:
		virtual void UpdateParam(const AT::Param& apParam) override {};
		virtual  void Start()override;
		virtual	void Stop() override;


	public:
		void OnMarketDepth(std::shared_ptr< AT::MarketData>);
		void OnMarketStatus(CTP_Market_Status_Enum aStatus,std::string aErrorMsg);

//		void SubSucribeAll();
	
	private:
		AT::IMarketSpi*	m_MarketSpi;
		std::unique_ptr<CTP::DepthReceiveV2> m_pDepth ;

	private: //config var
		std::set<std::string> m_SubscribeList;
		std::string m_Front;
		std::string m_CTP_WrokStreamDir;
		std::string m_ConfigFilePath;


	};
}
