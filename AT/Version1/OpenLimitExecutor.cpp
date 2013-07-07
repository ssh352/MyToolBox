#include "OpenLimitExecutor.h"
#include "IDriver_TD.h"
#include "../AT_Driver/ATLogger.h"
#include <sstream>
using namespace std;
namespace AT
{


OpenLimitExecutor::OpenLimitExecutor(int ValidTimeInSecond,int iBufferPoint)
	: m_OrderValidTime(ValidTimeInSecond)
	,m_iBufferPoint(iBufferPoint)
{
	m_StartTime = AT_INVALID_TIME;
}

OpenLimitExecutor::OpenLimitExecutor( const std::string& aConfigFile )
{
	InitFromConfigFile(aConfigFile);
}

void OpenLimitExecutor::InitFromConfigFile( const std::string& aConfigFile )
{
	m_OrderValidTime
}


OpenLimitExecutor::~OpenLimitExecutor(void)
{
}

std::string OpenLimitExecutor::GetExecutorID()
{
	return "LimitOpen";
}

Command OpenLimitExecutor::AddTarget( int addTargetQuantity, bool isBuy,const AT::MarketData& aMarket )
{
	if(addTargetQuantity == 0 )
	{
		Command lret;
		lret.reset(new InvalidCommand);
		return lret;
	}
	
	m_TragetVol = addTargetQuantity;
	m_IsBuy = isBuy;
	m_StartTime = aMarket.m_UpdateTime;
	m_InstrumentID = aMarket.InstrumentID;

	return PlaceOrder(addTargetQuantity,aMarket.m_LastPrice);
}

Command OpenLimitExecutor::OnMarketDepth( const AT::MarketData& aMarketDepth )
{
	m_LastMarket = aMarketDepth;
	if(m_LastMarket.m_UpdateTime - m_StartTime > boost::posix_time::seconds(m_OrderValidTime)
		&& m_StartTime != AT_INVALID_TIME)
	{
		//cancel order
		Command lret;
		CancelCommand* lpCancleOrder = new CancelCommand;

		lpCancleOrder->m_operation.m_Key = *m_SendOrderSet.rbegin();
		m_DelOrderSet = lpCancleOrder->m_operation.m_Key;
		lret.reset(lpCancleOrder);
		m_StartTime = AT_INVALID_TIME;
		m_TradeReport(0,0,m_IsBuy,true);
		return lret;
	}
	{
		Command lret;
		lret.reset(new InvalidCommand);
		return lret;
	}
}

Command OpenLimitExecutor::OnRtnTrade( const AT::TradeUpdate& apTrade )
{
	if (m_SendOrderSet.find(apTrade.m_Key ) != m_SendOrderSet.end())//单子成交
	{
		m_TragetVol -= apTrade.m_TradeVol;
		if(m_TragetVol == 0)
		{
			m_StartTime = AT_INVALID_TIME;
		}
		m_TradeReport(apTrade.m_TradePrice,apTrade.m_TradeVol,m_IsBuy,m_TragetVol == 0);		
	}

	{
		Command lret;
		lret.reset(new InvalidCommand);
		return lret;
	}
}

Command OpenLimitExecutor::OnRtnOrder( const AT::OrderUpdate& apOrder )
{
	if(apOrder.m_Key  == m_DelOrderSet && apOrder.m_OrderStatus == OrderStatusType::StoppedOrder)//撤单成功
	{
		m_StartTime = AT_INVALID_TIME;
		//return PlaceOrder(apOrder.m_Vol,m_IsBuy?m_LastMarket.m_AskPrice:m_LastMarket.m_BidPrice);
	}
	
	{
		Command lret;
		lret.reset(new InvalidCommand);
		return lret;
	}
}

Command OpenLimitExecutor::SetupTarget( int targetQuantity,bool isBuy, const AT::MarketData& aMarket )
{
	//if(addTargetQuantity == 0 )
	{
		Command lret;
		lret.reset(new InvalidCommand);
		return lret;
	}
}
Command OpenLimitExecutor::PlaceOrder(int addTargetQuantity,int Price)
{
	Command lret;
	InputCommand* lpInputOrder = new InputCommand;

	lpInputOrder->m_operation.m_Price = Price+m_iBufferPoint*100;
	lpInputOrder->m_operation.m_OpenCloseType = OpenCloseType::OpenOrder;
	lpInputOrder->m_operation.m_BuySellType = m_IsBuy? BuySellType::BuyOrder : BuySellType::SellOrder;
	lpInputOrder->m_operation.m_Vol = addTargetQuantity;
	strncpy_s(lpInputOrder->m_operation.InstrumentID , cInstrimentIDLength,m_InstrumentID.c_str(),cInstrimentIDLength);
	lpInputOrder->m_operation.m_Key = GenerateOrderKey();
	//市价、限价
	lpInputOrder->m_operation.m_OrderType = OrderPriceType::LimitOrder;

	lret.reset(lpInputOrder);
	m_SendOrderSet.insert(lpInputOrder->m_operation.m_Key);
	return lret;
}

}