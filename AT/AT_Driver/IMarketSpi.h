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
		virtual void OnRtnState(int aErrorCode,const std::string& aErrorMsg);
	public:
		virtual void OnRtnExchange(const std::string& aExchange);
		virtual void OnRtnProduct(const std::string& aProduct);
		virtual void OnRtnInstrument(const std::string& aInstrument);
		virtual void OnRtnMarketDepth(const std::string& aMarketDepth);
	};
}
