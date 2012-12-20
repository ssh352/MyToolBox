#pragma once

#include <string>
#include "AT_Error_Code.h"

namespace AT
{
	//not dispathe user keep thread safe
	class IMarketSpi
	{
	public:
		IMarketSpi();
		virtual ~IMarketSpi();

	public:
		virtual void NotifyState(int aErrorCode,const std::string& aErrorMsg);
	public:
		virtual void NotifyExchange(const std::string& aExchange);
		virtual void NotifyProduct(const std::string& aProduct);
		virtual void NotifyInstrument(const std::string& aInstrument);
		virtual void NotifyMarketDepth(const std::string& aMarketDepth);
	//public:
	//	virtual void NotifyAdvMarket(const std::string& aAdvanceMarketInfo);
	};
}
