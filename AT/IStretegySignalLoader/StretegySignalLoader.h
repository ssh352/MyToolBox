#pragma once
#include "IStrategy.h"
#include <Vector>
#include <windows.h>
#include <MarketCache.h>
namespace AT
{
	class IDriver_TD;
	class IStrategySpi;
	class ISignalModule;

	class StretegySignalLoader :public AT::IStrategy
	{
	public:
		StretegySignalLoader(const std::string& aConfigFile,AT::IDriver_TD * apTD  , AT::IStrategySpi* apStrSpi);
		virtual ~StretegySignalLoader(void);

	public:

		virtual void UpdateParam(EStrInputState errCode ,const std::string& apParam) override{};
		virtual void Start() override ;
		virtual void Stop()  override ;
	public:
		virtual void OnMarketDepth(const std::string& aMarketDepth)override;
		virtual void OnRtnOrder(const std::string& apOrder)override {};
		virtual void OnRtnTrade(const std::string& apTrade)override {};
		virtual void OnRtnPos(const std::string& aPos)override {};

	private:
		std::vector<AT::ISignalModule*>  m_SingleModuleVec;
		std::vector<HMODULE>				m_LibHandleVec;
		std::vector<std::pair< std::string, std::string> > m_SingleList;
		AT::MarketCache						m_MarketCache;
	};

}

