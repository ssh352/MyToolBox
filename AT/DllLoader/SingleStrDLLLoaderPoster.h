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

	//todo  constuct from AT::IStrategy* and remove setup remove driverd from IStrategySpi
	class SingleStrDLLLoaderPoster 
		:public IMarketSpi 
		, public ITradeSpi
		, public IStrategySpi
	{
	public:
		SingleStrDLLLoaderPoster(std::atomic<bool>&  aFinishedFalg);
		virtual ~SingleStrDLLLoaderPoster(void);


		public:
			void Setup( AT::IStrategy* apStr)
			{
				m_pStr = apStr;
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
		virtual void NotifyStateStr(EStrState ErrorCode,const char* aErrorMsg)
		{
			std::cout<<aErrorMsg;
		}

		AT::IStrategy* m_pStr;
		std::atomic<bool>&  m_FinishedFalg;


	};

}

