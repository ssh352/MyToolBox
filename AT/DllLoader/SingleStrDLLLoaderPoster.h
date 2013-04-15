#pragma once
#include "IMarketSpi.h"
#include "ITradeSpi.h"
#include "IStrategySpi.h"
#include <iostream>
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
		SingleStrDLLLoaderPoster(void);
		virtual ~SingleStrDLLLoaderPoster(void);


		public:
			void Setup( AT::IStrategy* apStr)
			{
				m_pStr = apStr;
			}
		//Market
	public:
		virtual void NotifyStateMD(EMarketState aErrorCode,const std::string& aErrorMsg) override;
		virtual void NotifyExchange(const std::string& aExchange) override;
		virtual void NotifyProduct(const std::string& aProduct) override;
		virtual void NotifyInstrument(const std::string& aInstrument) override;
		virtual void NotifyMarketDepth(const std::string& aMarketDepth) override;

		//TD
	public:
		virtual void NotifyStateTD(ETradeState aErrorCode,const std::string& aErrorMsg) override ;
		virtual void OnRtnOrder(const std::string& apOrder)override ;
		virtual void OnRtnTrade(const std::string& apTrade) override;
		virtual void OnRtnPosition(const std::string& aPosition) override;

		//Str
		virtual void NotifyStateStr(EStrState ErrorCode,const std::string& aErrorMsg) override
		{
			std::cout<< aErrorMsg<<'\n';
		}

		AT::IStrategy* m_pStr;


	};

}

