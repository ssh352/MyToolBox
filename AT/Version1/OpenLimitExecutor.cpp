#include "OpenLimitExecutor.h"
#include "IDriver_TD.h"
namespace AT
{


OpenLimitExecutor::OpenLimitExecutor(int ValidTimeInSecond)
	: m_OrderValidTime(ValidTimeInSecond)
{
}


OpenLimitExecutor::~OpenLimitExecutor(void)
{
}

std::string OpenLimitExecutor::GetExecutorID()
{
	return "LimitOpen";
}

boost::shared_ptr<TradeCommand> OpenLimitExecutor::AddTarget( int addTargetQuantity, bool isBuy,const AT::MarketData& aMarket )
{
	if(addTargetQuantity == 0 )
	{
		boost::shared_ptr<TradeCommand> lret;
		lret.reset(new InvalidCommand);
		return lret;
	}
	boost::shared_ptr<TradeCommand> lret;
	InputCommand* lpInputOrder = new InputCommand;

	lpInputOrder->m_operation.m_Price = aMarket.m_LastPrice;
	lpInputOrder->m_operation.m_OpenCloseType = OpenCloseType::OpenOrder;
	lpInputOrder->m_operation.m_BuySellType = isBuy? BuySellType::BuyOrder : BuySellType::SellOrder;
	lpInputOrder->m_operation.m_Vol = addTargetQuantity;
	strncpy_s(lpInputOrder->m_operation.InstrumentID , cInstrimentIDLength,aMarket.InstrumentID,cInstrimentIDLength);
	lpInputOrder->m_operation.m_Key = GenerateOrderKey();
	//�мۡ��޼�
	lpInputOrder->m_operation.m_OrderType = OrderType::MarketOrder;

	lret.reset(lpInputOrder);
	m_SendOrderSet.insert(lpInputOrder->m_operation.m_Key);
	m_TragetVol = addTargetQuantity;
	m_IsBuy = isBuy;
	m_StartTime = aMarket.m_UpdateTime;
	m_OrderType = lpInputOrder->m_operation.m_OrderType;
	return lret;
}

boost::shared_ptr<TradeCommand> OpenLimitExecutor::OnMarketDepth( const AT::MarketData& aMarketDepth )
{
	m_LastMarket = aMarketDepth;
	if(
		m_LastMarket.m_UpdateTime - m_StartTime > boost::posix_time::seconds(m_OrderValidTime)
		&& m_TragetVol != 0 )
	{
		//cancel order
		boost::shared_ptr<TradeCommand> lret;
		CancelCommand* lpCancleOrder = new CancelCommand;

		lpCancleOrder->m_operation.m_Key = *m_SendOrderSet.rbegin();
		m_DelOrderSet = lpCancleOrder->m_operation.m_Key;
		lret.reset(lpCancleOrder);
		m_FinishehNotfiy(0,0,m_IsBuy,true);
		return lret;
	}
	{
		boost::shared_ptr<TradeCommand> lret;
		lret.reset(new InvalidCommand);
		return lret;
	}
}

boost::shared_ptr<TradeCommand> OpenLimitExecutor::OnRtnTrade( const AT::TradeUpdate& apTrade )
{
	if (m_SendOrderSet.find(apTrade.m_Key ) != m_SendOrderSet.end())
	{
		m_TragetVol -= apTrade.m_TradeVol;
		m_FinishehNotfiy(apTrade.m_TradePrice,apTrade.m_TradeVol,m_IsBuy,m_TragetVol == 0);		
	}

	{
		boost::shared_ptr<TradeCommand> lret;
		lret.reset(new InvalidCommand);
		return lret;
	}
}

boost::shared_ptr<TradeCommand> OpenLimitExecutor::OnRtnOrder( const AT::OrderUpdate& apOrder )
{
	if(apOrder.m_Key  == m_DelOrderSet && apOrder.m_OrderStatus == OrderStatusType::StoppedOrder)
	{
		m_TragetVol -= apOrder.m_Vol;
		return AddOrder(apOrder.m_Vol);
	}
	
	{
		boost::shared_ptr<TradeCommand> lret;
		lret.reset(new InvalidCommand);
		return lret;
	}
}

boost::shared_ptr<TradeCommand> OpenLimitExecutor::SetupTarget( int targetQuantity,bool isBuy, const AT::MarketData& aMarket )
{
	//if(addTargetQuantity == 0 )
	{
		boost::shared_ptr<TradeCommand> lret;
		lret.reset(new InvalidCommand);
		return lret;
	}
}
boost::shared_ptr<TradeCommand> OpenLimitExecutor::AddOrder(int vol)
{
	boost::shared_ptr<TradeCommand> lret;
	InputCommand* lpInputOrder = new InputCommand;

	lpInputOrder->m_operation.m_Price = m_IsBuy?m_LastMarket.m_AskPrice:m_LastMarket.m_BidPrice;
	lpInputOrder->m_operation.m_OpenCloseType = OpenCloseType::OpenOrder;
	lpInputOrder->m_operation.m_BuySellType = m_IsBuy? BuySellType::BuyOrder : BuySellType::SellOrder;
	lpInputOrder->m_operation.m_Vol = vol;
	strncpy_s(lpInputOrder->m_operation.InstrumentID , cInstrimentIDLength,m_LastMarket.InstrumentID,cInstrimentIDLength);
	lpInputOrder->m_operation.m_Key = GenerateOrderKey();
	//�мۡ��޼�
	lpInputOrder->m_operation.m_OrderType = OrderType::MarketOrder;

	lret.reset(lpInputOrder);
	m_SendOrderSet.insert(lpInputOrder->m_operation.m_Key);
	m_TragetVol = vol;
	m_StartTime = m_LastMarket.m_UpdateTime;
	m_OrderType = lpInputOrder->m_operation.m_OrderType;
	return lret;
}

}