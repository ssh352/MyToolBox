#pragma once
#include "IMarketSpi.h"
#include "ITradeSpi.h"
#include "IStrategySpi.h"
#include <iostream>
#include <atomic>

namespace AT
{
	class IDriver_MD;
	class IDriver_TD;
	class IStrategy;
	class IMarketCache;


	class main_versionLoaderPoster
		:public IMarketSpi 
		, public ITradeSpi
		, public IStrategySpi
	{
	public:
		main_versionLoaderPoster(std::atomic<bool>&  aFinishedFalg);
		~main_versionLoaderPoster(void);

	public:
		void Setup( AT::IStrategy* apStr)
		{
			m_pStrategy = apStr;
		}

		//Market
	public:
		virtual void NotifyStateMD(EMarketState aErrorCode,const char* aErrorMsg) override;
		virtual void NotifyMarketDepth(const MarketData& aMarketDepth) override;

		//TD
	public:


		virtual void NotifyStateTD(ETradeState aErrorCode,const char* aErrorMsg) override ;
		virtual void OnRtnOrder(const OrderUpdate& apOrder)override ;
		virtual void OnRtnTrade(const TradeUpdate& apTrade) override;

		//Str
		virtual void NotifyStateStr(EStrState ErrorCode,const char* aErrorMsg);

	private:
		AT::IStrategy* m_pStrategy;
		std::atomic<bool>&  m_FinishedFalg;
	};
}


