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

		//ע��۲���
		void RegisterObserver(ITradeAccountObserver*);
	};
}