#pragma once

#include <string>
#include "AT_Error_Code.h"

namespace AT
{
	class ITradeSpi
	{
	public:
		ITradeSpi();
		virtual ~ITradeSpi();

	public:
		virtual void OnRtnState(int aErrorCode,const std::string& aErrorMsg);
		virtual void OnRtnOrder(const std::string& apOrder);
		virtual void OnRtnTrade(const std::string& apTrade);
		virtual void OnRtnPosition(const std::string& aPosition);
	};
}

