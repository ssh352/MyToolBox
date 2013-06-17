#pragma  once
#include "TradeSignal.h"
#include "AT_Struct.h"
namespace AT
{
	class ITradeAccount
	{
	public:
		virtual void HandleTradeSignal(const TradeSignal& aTradeSignal) = 0;
		virtual void OnMarketDepth(const MarketData& aMarketDepth) = 0;
		virtual void OnRtnOrder(const  OrderUpdate& apOrder) = 0;
		virtual void OnRtnTrade(const  TradeUpdate& apTrade) = 0;  
	};
}