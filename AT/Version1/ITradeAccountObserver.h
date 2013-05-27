
#pragma  once
#include "AT_Struct.h"
namespace AT
{
	class ITradeAccount;
	class ITradeAccountObserver
	{
	public:
		void NotifyAccoutStateChange(const AT::OrderUpdate& aOrder, AT::ITradeAccount* );
	};

}