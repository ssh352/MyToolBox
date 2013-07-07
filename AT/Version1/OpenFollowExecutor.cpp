#include "OpenFollowExecutor.h"
#include "IDriver_TD.h"
#include "../AT_Driver/ATLogger.h"
namespace AT
{


OpenFollowExecutor::OpenFollowExecutor( FollowExecutorParma aParma )
	:m_Setting(aParma)
{
	m_ExecutorStatus = FollowExecutorStatus::IDLE;
}

OpenFollowExecutor::OpenFollowExecutor( const std::string&  configFile )
{

}


OpenFollowExecutor::~OpenFollowExecutor(void)
{
}

std::string OpenFollowExecutor::GetExecutorID()
{
	return "MarketOpen";
}

Command OpenFollowExecutor::AddTarget( int addTargetQuantity, bool isBuy,const AT::MarketData& aMarket )
{
	if(addTargetQuantity == 0 )
	{
		Command lret;
		lret.reset(new InvalidCommand);
		return lret;
	}
	
	m_StartTime = aMarket.m_UpdateTime;
	m_IsBuy = isBuy;
	m_InstrumentID = aMarket.InstrumentID;

	m_LastPrice = aMarket.m_LastPrice;
	
	return PlaceOrder(addTargetQuantity);
}

Command OpenFollowExecutor::OnMarketDepth( const AT::MarketData& aMarketDepth )
{
	m_LastPrice = aMarketDepth.m_LastPrice;
	if(m_ExecutorStatus == FollowExecutorStatus::WaitingOrderPlace)
	{
		if(IsOrderPriceNeedModify(aMarketDepth))
		{	
			Command lret;
			CancelCommand* lpCancleOrder = new CancelCommand;
			lpCancleOrder->m_operation.m_Key = m_LastSendOrderKey;
			lret.reset(lpCancleOrder);
			m_ExecutorStatus = FollowExecutorStatus::WaitingOrderCancel;
			return lret;
		}

	}

	{
		Command lret;
		lret.reset(new InvalidCommand);
		return lret;
	}
}

Command OpenFollowExecutor::OnRtnTrade( const AT::TradeUpdate& apTrade )
{
	if (m_SendOrderSet.find(apTrade.m_Key ) != m_SendOrderSet.end())
	{
		m_TragetVol -= apTrade.m_TradeVol;
		m_TradeReport(apTrade.m_TradePrice,apTrade.m_TradeVol,m_IsBuy,m_TragetVol == 0);		
	}

	{
		Command lret;
		lret.reset(new InvalidCommand);
		return lret;
	}
}

Command OpenFollowExecutor::OnRtnOrder( const AT::OrderUpdate& apOrder )
{

	if (apOrder.m_Key == m_LastSendOrderKey)
	{
		if (m_ExecutorStatus == FollowExecutorStatus::WaitingOrderPlace)
		{
			//assert (apOrder.m_OrderStatus == OrderStatusType::ActiveOrder)
			m_ExecutorStatus = FollowExecutorStatus::OrderPlaced;
		} 
		else if (m_ExecutorStatus == FollowExecutorStatus::WaitingOrderCancel)
		{
			m_ExecutorStatus = FollowExecutorStatus::WaitingOrderPlace;
			return PlaceOrder( m_TragetVol);;
		}

	}
	{
		Command lret;
		lret.reset(new InvalidCommand);
		return lret;
	}
}

Command OpenFollowExecutor::SetupTarget( int targetQuantity,bool isBuy, const AT::MarketData& aMarket )
{
	return AddTarget(targetQuantity,isBuy,aMarket);
}

bool OpenFollowExecutor::IsOrderPriceNeedModify(const AT::MarketData& aMarketDepth)
{
	int iDiffPrice = abs(m_LastOrderPrice-aMarketDepth.m_LastPrice);
	if(iDiffPrice < m_Setting.m_FollowRange)
	{
		return true;
	}
	return false;
}

Command OpenFollowExecutor::PlaceOrder( int addTargetQuantity)
{
	Command lret;
	InputCommand* lpInputOrder = new InputCommand;

	lpInputOrder->m_operation.m_Price = m_LastPrice;
	lpInputOrder->m_operation.m_OpenCloseType = OpenCloseType::OpenOrder;
	lpInputOrder->m_operation.m_BuySellType = m_IsBuy? BuySellType::BuyOrder : BuySellType::SellOrder;
	lpInputOrder->m_operation.m_Vol = addTargetQuantity;
	strncpy_s(lpInputOrder->m_operation.InstrumentID , cInstrimentIDLength,m_InstrumentID.c_str(),cInstrimentIDLength);

	lpInputOrder->m_operation.m_Key = GenerateOrderKey();

	lpInputOrder->m_operation.m_OrderType = OrderPriceType::LimitOrder;

	lret.reset(lpInputOrder);

	m_SendOrderSet.insert( lpInputOrder->m_operation.m_Key);
	m_LastSendOrderKey = lpInputOrder->m_operation.m_Key;
	m_LastSendOrderPrice = lpInputOrder->m_operation.m_Price;
	m_TragetVol = addTargetQuantity;

	m_ExecutorStatus = FollowExecutorStatus::WaitingOrderPlace;
	m_LastOrderPrice = m_LastPrice;
	return lret;
}

}