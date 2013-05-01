#pragma once
#include "AT_Struct.h"

namespace AT
{
	enum class EMarketState
	{
		START,
		READY,
		WARN_MarketDelay,
		END_MARKETDAY,
		STOP,
	};
	class IMarketSpi
	{

	public:
		virtual void NotifyStateMD(EMarketState aErrorCode,const char* aErrorMsg) = 0;
	public:
		//virtual void NotifyExchange(const std::string& aExchange) = 0;
		//virtual void NotifyProduct(const std::string& aProduct) = 0 ;
		//virtual void NotifyInstrument(const std::string& aInstrument)= 0;
		virtual void NotifyMarketDepth(const MarketData& aMarketDepth)= 0;

	};
}
