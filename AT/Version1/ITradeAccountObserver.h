
#pragma  once
#include "AT_Struct.h"
namespace AT
{

	class ITradeAccountObserver;

	enum class TradeAccountOrderStatus
	{
		NoOrder,
		OpenOrder,
		CloseOrder
	}; 

	enum class TradeAccountPositionStatus
	{
		Empty_Pos,
		Partly_Pos,
		Full_Pos
	}; 

	class ITradeAccountObserver
	{
	public:
		virtual void NotifyAccoutStateChange(TradeAccountOrderStatus aOrderSatus, TradeAccountPositionStatus aPosStatus,const std::string& accountID) = 0;
	};

}