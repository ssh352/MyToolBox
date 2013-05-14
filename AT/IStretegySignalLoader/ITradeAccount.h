#pragma  once
#include "TradeCommand.h"
namespace AT
{
	class ITradeAccountObserver;

	class ITradeAccount
	{
	public:
		//input method call from TradeSignalExecutor
		void HandleTradeCommand(const AT::TradeCommand& aSignal);

		//×¢²á¹Û²ìÕß
		void RegisterObserver(ITradeAccountObserver*);
	};
}