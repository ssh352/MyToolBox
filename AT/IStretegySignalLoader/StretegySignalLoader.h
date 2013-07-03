#pragma once
#include "IStrategy.h"
#include <Vector>
#include <windows.h>
#include <MarketCache.h>
namespace AT
{
	class IDriver_TD;
	class IStrategySpi;
	class IIndexModule;
	class IMarketCache;

	class StretegySignalLoader :public AT::IStrategy
	{
	public:
		StretegySignalLoader(const char* aConfigFile,AT::IDriver_TD * apTD  , AT::IStrategySpi* apStrSpi,const AT::IMarketCache* apMarket);
		virtual ~StretegySignalLoader(void);

		virtual void UpdateParam(EStrInputState errCode ,const Param& apParam) override{} ;
		virtual void Start()override ;
		virtual void Stop()override;
	public:
		virtual void OnMarketDepth(const MarketData& aMarketDepth)override;
		virtual void OnRtnOrder(const  OrderUpdate& apOrder)override{};
		virtual void OnRtnTrade(const  TradeUpdate& apTrade)override{};

	private:
		std::vector<AT::IIndexModule*>  m_SingleModuleVec;
		std::vector<HMODULE>				m_LibHandleVec;
		std::vector<std::pair< std::string, std::string> > m_SingleList;
		const AT::IMarketCache*						m_pMarketCache;
	};

}

