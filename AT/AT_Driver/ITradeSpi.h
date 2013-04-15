#pragma once

#include <string>
namespace AT
{
	enum class ETradeState
	{
		START,
		READY,
		STOP,
	};

	class ITradeSpi
	{
	public:
		virtual void NotifyStateTD(ETradeState aErrorCode,const std::string& aErrorMsg) = 0;
		virtual void OnRtnOrder(const std::string& apOrder) = 0;
		virtual void OnRtnTrade(const std::string& apTrade)=0;
		virtual void OnRtnPosition(const std::string& aPosition)=0;
	};
}

