#pragma once
#include <string>
namespace AT
{

	class IStrategySpi
	{
		enum EStrState
		{
			START,
			STOP,
			INFO
		};
		//for notify
		virtual void NotifyState(EStrState ErrorCode,const std::string& aErrorMsg){};
	};
}