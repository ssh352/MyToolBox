#pragma once
#include <string>
namespace AT
{
	enum class EStrState
	{
		START,
		STOP,
		INFO
	};
	class IStrategySpi
	{

		//for notify 
		virtual void NotifyStateStr(EStrState ErrorCode,const std::string& aErrorMsg) = 0;
	};
}