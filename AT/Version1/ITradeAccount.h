#pragma  once
#include "TradeSignal.h"
#include "AT_Struct.h"
#include <boost\function.hpp>
namespace AT
{
	class ITradeAccount;
	typedef boost::function<void (int32_t aProfit,AT_Time aTime ,ITradeAccount* sender ) > TradeSignalProfitTimeNotifyer;
	class ITradeAccount
	{
	public:
		virtual void HandleTradeSignal(const TradeSignal& aTradeSignal) = 0;
		virtual void OnMarketDepth(const MarketData& aMarketDepth) = 0;
		virtual void OnRtnOrder(const  OrderUpdate& apOrder) = 0;
		virtual void OnRtnTrade(const  TradeUpdate& apTrade) = 0;  
		virtual void SetProfitCallback(TradeSignalProfitTimeNotifyer aTradeSignalProfitTimeNotifyerCallback) = 0; 
	};
}