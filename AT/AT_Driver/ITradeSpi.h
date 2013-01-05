#pragma once

#include <string>
#include "AT_Error_Code.h"

namespace AT
{
	class ITradeSpi
	{
	public:
		virtual void OnRtnState(int aErrorCode,const std::string& aErrorMsg) = 0;
		virtual void OnRtnOrder(const std::string& apOrder) = 0;
		virtual void OnRtnTrade(const std::string& apTrade)=0;
		virtual void OnRtnPosition(const std::string& aPosition)=0;
	};
}

