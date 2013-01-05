#pragma once

#include <string>
#include "AT_Error_Code.h"

namespace AT
{
	//not dispathe user keep thread safe
	class IMarketSpi
	{
	public:
		virtual void NotifyState(int aErrorCode,const std::string& aErrorMsg) = 0;
	public:
		virtual void NotifyExchange(const std::string& aExchange) = 0;
		virtual void NotifyProduct(const std::string& aProduct) = 0 ;
		virtual void NotifyInstrument(const std::string& aInstrument)= 0;
		virtual void NotifyMarketDepth(const std::string& aMarketDepth)= 0;
	//public:
	//	virtual void NotifyAdvMarket(const std::string& aAdvanceMarketInfo);
	};
}
