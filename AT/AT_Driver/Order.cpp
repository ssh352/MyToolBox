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
		aOrderUpdate.m_OrderType = aSrcOrder.m_OrderType;
		aOrderUpdate.m_OpenCloseType = aSrcOrder.m_OpenCloseType;
		aOrderUpdate.m_Key = aSrcOrder.m_Key;
	}




	std::string InputOrder::ToString() const
	{

		return str(boost::format("InputOrder [OrderKey: %s] \n [InstrumentID:: %s] [AccoutID:: %s] \n [Price: %d]  [Vol: %u] [BuySell: %s] [OrderType: %s][OpenClose :%s]\n\
								 [MoreInfo: %s] \n [SenderInfo: %s]") 
			% AT::ToString(m_Key)
			% InstrumentID % AccoutID
			% m_Price %m_Vol
			%   AT::ToString (m_BuySellType) 
			%	AT::ToString (m_OrderType)
			%	AT::ToString (m_OpenCloseType) 
			% m_MoreInfo % m_SenderInfo
			);
	}


	std::string CancelOrder::ToString() const
	{
		return str(boost::format("CancleOrder [OrderKey: %s] \n")
								 % AT::ToString(m_Key)
								 );
	}


	std::string ModifyOrder::ToString() const
	{
		return str(boost::format("ModifyOrder [OrderKey: %s] \n [Price: %d] [Vol: %u] ") 
			% AT::ToString(m_Key)
			% m_Price %m_Vol
			);
	}


	std::string OrderUpdate::ToString() const
	{

		return str(boost::format("OrderUpdate [OrderKey: %s] \n[InstrumentID:: %s] [AccoutID:: %s] [ExchangeID: %s] \n[Price: %d]  [Vol: %u] [BuySell: %s] [OrderType: %s][OpenClose :%s] [TradeVol: %d] [LiveVol: %d]\
								 [Time %s] [OrderStatus: %s]") 
								 % AT::ToString(m_Key)
								 % InstrumentID % AccoutID % m_PlatformOrderID
								 % m_Price %m_Vol
								 %  AT::ToString(m_BuySellType)
								 %  AT::ToString(m_OrderType) 
								 %  AT::ToString(m_OpenCloseType) 
								 %  m_TradedVol % m_LiveVol
								 %  AT::ToString(m_UpdateTime) % AT::ToString(m_OrderStatus)
								 );
	}

}


