#include "Order.h"
#include <boost\format.hpp>
namespace AT
{
	void CopyOrderFields( OrderUpdate& aOrderUpdate,const InputOrder& aSrcOrder )
	{
		memcpy(aOrderUpdate.AccoutID , aSrcOrder.AccoutID,sizeof(aOrderUpdate.AccoutID));
		memcpy(aOrderUpdate.InstrumentID , aSrcOrder.InstrumentID,sizeof(aOrderUpdate.InstrumentID));

		aOrderUpdate.m_Price = aSrcOrder.m_Price;
		aOrderUpdate.m_Vol = aSrcOrder.m_Vol;

		aOrderUpdate.m_BuySellType = aSrcOrder.m_BuySellType;
		aOrderUpdate.m_OrderType = aSrcOrder.m_OrderPriceType;
		aOrderUpdate.m_OpenCloseType = aSrcOrder.m_OpenCloseType;
		aOrderUpdate.m_Key = aSrcOrder.m_Key;
	}

	std::string ToString( const InputOrder& aInputOrder )
	{
		return str(boost::format("InputOrder [OrderKey: %s] \n [InstrumentID:: %s] [AccoutID:: %s] \n [Price: %d]  [Vol: %u] [BuySell: %s] [OrderType: %s][OpenClose :%s]\n\
								 [TimeType %s] [Time %s] [TriggerType %s] [TriggerPrice %s]\
								[MoreInfo: %s] \n [SenderInfo: %s]") 
								 % AT::ToString(aInputOrder.m_Key)
								 % aInputOrder.InstrumentID % aInputOrder.AccoutID
								 % aInputOrder.m_Price %aInputOrder.m_Vol
								 %   AT::ToString (aInputOrder.m_BuySellType) 
								 %	AT::ToString (aInputOrder.m_OrderPriceType)
								 %	AT::ToString (aInputOrder.m_OpenCloseType) 
								 % AT::ToString(aInputOrder. m_TimeInForceCode) %aInputOrder.m_GTDDate %AT::ToString(aInputOrder.m_TriggerType) % aInputOrder.m_TriggerPrice
								 % aInputOrder.m_MoreInfo % aInputOrder.m_SenderInfo
								 );
	}

	std::string ToString( const CancelOrder& aCancelOrder )
	{
		return str(boost::format("CancleOrder [OrderKey: %s] \n")
			% AT::ToString(aCancelOrder.m_Key)
			);
	}

	std::string ToString( const ModifyOrder& aModifyROrder )
	{
		return str(boost::format("ModifyOrder [OrderKey: %s] \n [Price: %d] [Vol: %u] ") 
			% AT::ToString(aModifyROrder.m_Key)
			% aModifyROrder.m_Price % aModifyROrder.m_Vol
			);
	}

	std::string ToString( const OrderUpdate& aOrderUpdate )
	{
		return str(boost::format("OrderUpdate [OrderKey: %s] \n[InstrumentID:: %s] [AccoutID:: %s] [ExchangeID: %s] \n[Price: %d]  [Vol: %u] [BuySell: %s] [OrderType: %s][OpenClose :%s] [TradeVol: %d] [LiveVol: %d]\
								 [Time %s] [OrderStatus: %s] [OrderSubmitStatus %s]") 
								 % AT::ToString(aOrderUpdate.m_Key)
								 % aOrderUpdate.InstrumentID % aOrderUpdate.AccoutID % aOrderUpdate.m_PlatformOrderID
								 % aOrderUpdate.m_Price %aOrderUpdate.m_Vol
								 %  AT::ToString(aOrderUpdate.m_BuySellType)
								 %  AT::ToString(aOrderUpdate.m_OrderType) 
								 %  AT::ToString(aOrderUpdate.m_OpenCloseType) 
								 % aOrderUpdate. m_TradedVol % aOrderUpdate.m_LiveVol
								 %  AT::ToString(aOrderUpdate.m_UpdateTime) % AT::ToString(aOrderUpdate.m_OrderStatus) % AT::ToString(aOrderUpdate.m_OrderSubmitState)
								 );
	}

}


