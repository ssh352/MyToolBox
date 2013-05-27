#pragma  once
#include "TradeCommand.h"
namespace AT
{
	class ITradeAccountObserver;

	class ITradeAccount
	{
	public:
		//input method call from TradeSignalExecutor
		virtual void HandleTradeCommand(const AT::TradeCommand& aTradeCommand) = 0;

		//ע��۲���
		virtual void RegisterObserver(ITradeAccountObserver*) = 0;

		enum  class TradeAccountState
		{
			IDLE,
			BUSY,
		};
	};
}