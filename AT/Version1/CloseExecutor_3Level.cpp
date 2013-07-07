#include "CloseExecutor_3Level.h"
#include "IDriver_TD.h"
#include "boost/format.hpp"
#include "../AT_Driver/ATLogger.h"
#include <sstream>
using namespace std;
namespace AT
{

CloseExecutor_3Level::~CloseExecutor_3Level(void)
{
}

CloseExecutor_3Level::CloseExecutor_3Level( CloseSetting_3Level aSetting)
	:m_Setting(aSetting)
	,m_CurrentLevel(0)
	,m_Status(Status::IDLE)
{
	m_TargetVol = 0;
	m_ActiveOrderVol = 0;
	m_TriggerLossTime = AT_INVALID_TIME;
}

CloseExecutor_3Level::CloseExecutor_3Level( const std::string& aConfigFile )
{

}

boost::shared_ptr<TradeCommand> CloseExecutor_3Level::SetupTarget( int targetQuantity, bool isBuy, const AT::MarketData& aMarket )
{
	return AddTarget(targetQuantity,isBuy,aMarket);
}

boost::shared_ptr<TradeCommand> CloseExecutor_3Level::AddTarget( int addTargetQuantity,bool isBuy, const AT::MarketData& aMarket )
{
	if(addTargetQuantity == 0 )
	{
		boost::shared_ptr<TradeCommand> lret;
		lret.reset(new InvalidCommand);
		return lret;
	}

	m_TargetVol += addTargetQuantity;
	m_IsBuy = isBuy;
	m_StartPrice = aMarket.m_LastPrice;

	//暂时注释掉，等待明确的解释
	//SetSignalDirectionVol(isBuy?BuySellType::BuyOrder:BuySellType::SellOrder,m_TargetVol);

	return PlaceStopLossOrder(aMarket);

}

boost::shared_ptr<TradeCommand> CloseExecutor_3Level::OnMarketDepth( const AT::MarketData& aMarketDepth )
{
	m_LastMarket = aMarketDepth;
	if(m_TargetVol > 0 ||
		(m_TargetVol == 0 && m_ActiveOrderVol > 0))
	{
		if(aMarketDepth.m_UpdateTime>= m_Setting.StopClearTime)//市价清仓
		{
			return DoQuit(aMarketDepth);
		}
		if(aMarketDepth.m_UpdateTime >= m_Setting.StopTime)//全部清仓
		{
			return DoQuit(aMarketDepth);
		}
		
		int lPriceDiffStart = aMarketDepth.m_LastPrice - m_StartPrice;
		if(m_LastLossPrice >= aMarketDepth.m_LastPrice && m_TriggerLossTime == AT_INVALID_TIME)
		{
			m_TriggerLossTime = aMarketDepth.m_UpdateTime;
		}
		if((aMarketDepth.m_UpdateTime - m_TriggerLossTime)>boost::posix_time::seconds(3))
		{
			return DoQuit(aMarketDepth);
		}
		switch (m_CurrentLevel)
		{
		case 0:	
			if(m_LastLossPrice - aMarketDepth.m_LastPrice >2&&m_LastLossPrice != 0)//止损超过止损预案价格2点时，用市价清仓
			{
				return DoQuit(aMarketDepth);
			}
			else if (lPriceDiffStart >= m_Setting.EnterLevel_1)
			{
				 m_CurrentLevel = 1;
			}
			break;
		case  1:
			if(lPriceDiffStart < m_Setting.QuitLevel_1)
			{
				return DoQuit(aMarketDepth);
			}
			else if (lPriceDiffStart >= m_Setting.EnterLevel_2)
			{
				m_CurrentLevel = 2;
			}
			break;
		case 2:
			if(lPriceDiffStart < m_Setting.QuitLevel_2)
			{
				return DoQuit(aMarketDepth);
			}
			else if(lPriceDiffStart >= m_Setting.EnterLevel_3)
			{
				m_CurrentLevel = 3;
			}
			break;
		case 3:
			if(lPriceDiffStart <m_Setting.QuitLevel_3)
			{
				return DoQuit(aMarketDepth);
			}
			break;
		default:
			break;
		}
	}

	boost::shared_ptr<TradeCommand> lret;
	lret.reset(new InvalidCommand);
	return lret;
	
}

boost::shared_ptr<TradeCommand> CloseExecutor_3Level::DoQuit(const AT::MarketData& aMarket )
{
	boost::shared_ptr<TradeCommand> lret;

	if(m_Setting.QuitLevel_0 < 0)//有止损止盈单.必须先撤单，
	{
		boost::shared_ptr<TradeCommand> lret;
		CancelCommand* lpCancleOrder = new CancelCommand;

		lpCancleOrder->m_operation.m_Key = *m_SendOrderSet.rbegin();
		m_DelOrderSet = lpCancleOrder->m_operation.m_Key;
		lret.reset(lpCancleOrder);
		return lret;
	}
	
	return PlaceCloseOrder(aMarket);

}

boost::shared_ptr<TradeCommand> CloseExecutor_3Level::OnRtnOrder( const AT::OrderUpdate& apOrder )
{
	boost::shared_ptr<TradeCommand> lret;
	lret.reset(new InvalidCommand);

	if(m_SendOrderSet.find(apOrder.m_Key) == m_SendOrderSet.end())
	{
		return lret;
	}
	//撤单
	if(apOrder.m_Key  == m_DelOrderSet && apOrder.m_OrderStatus == OrderStatusType::StoppedOrder)
	{
		m_LastLossPrice = 0;
		return PlaceCloseOrder(m_LastMarket);
	}
	return lret;
}

boost::shared_ptr<TradeCommand> CloseExecutor_3Level::OnRtnTrade( const AT::TradeUpdate& apTrade )
{

	boost::shared_ptr<TradeCommand> lret;
	lret.reset(new InvalidCommand);

	if(m_SendOrderSet.find(apTrade.m_Key) == m_SendOrderSet.end())
	{
		return lret;
	}
	if(m_SendLossSet.find(apTrade.m_Key) != m_SendLossSet.end())
	{
		m_TargetVol -= apTrade.m_TradeVol;
	}

	m_ActiveOrderVol -= apTrade.m_TradeVol;

	bool isFinishe = m_ActiveOrderVol == 0 ;
	m_FinishehNotfiy(apTrade.m_TradePrice,apTrade.m_TradeVol,m_IsBuy,isFinishe);

	return lret;
}

std::string CloseExecutor_3Level::GetExecutorID()
{
	return "Level3Quit";
}

boost::shared_ptr<TradeCommand> CloseExecutor_3Level::PlaceStopLossOrder(const AT::MarketData& aMarket)
{
	boost::shared_ptr<TradeCommand> lret;
	InputCommand* lpInputOrder = new InputCommand;

	int lPriceDiffer = m_Setting.StopLossOrderPrice * 100  * (m_IsBuy ? 1 : -1);
	lpInputOrder->m_operation.m_Price = aMarket.m_LastPrice + lPriceDiffer;
	lpInputOrder->m_operation.m_OpenCloseType = OpenCloseType::CloseTodayOrder;
	lpInputOrder->m_operation.m_BuySellType = m_IsBuy? BuySellType::BuyOrder : BuySellType::SellOrder;
	lpInputOrder->m_operation.m_Vol = m_TargetVol;
	strncpy_s(lpInputOrder->m_operation.InstrumentID , cInstrimentIDLength,aMarket.InstrumentID,cInstrimentIDLength);
	lpInputOrder->m_operation.m_Key = GenerateOrderKey();
	lpInputOrder->m_operation.m_OrderType = OrderType::StopLossOrder;
	lret.reset(lpInputOrder);


	//m_SendOrderSet.insert(lpInputOrder->m_operation.m_Key);
	//m_SendLossSet.insert(lpInputOrder->m_operation.m_Key);
	//m_LastLossPrice = aMarket.m_LastPrice;
	//m_ActiveOrderVol += m_TargetVol;

	return lret;


}
boost::shared_ptr<TradeCommand> CloseExecutor_3Level::PlaceCloseOrder(const AT::MarketData& aMarket)
{
	boost::shared_ptr<TradeCommand> lret;
	InputCommand* lpInputOrder = new InputCommand;

	lpInputOrder->m_operation.m_Price = aMarket.m_LastPrice;
	lpInputOrder->m_operation.m_OpenCloseType = OpenCloseType::CloseTodayOrder;
	lpInputOrder->m_operation.m_BuySellType = m_IsBuy? BuySellType::BuyOrder : BuySellType::SellOrder;
	lpInputOrder->m_operation.m_Vol = m_TargetVol==0?m_ActiveOrderVol:m_TargetVol;
	if(m_TargetVol != 0)
	{
		m_ActiveOrderVol += m_TargetVol;
	}
	m_TargetVol = 0;
	m_CurrentLevel = 0;
	strncpy_s(lpInputOrder->m_operation.InstrumentID , cInstrimentIDLength,aMarket.InstrumentID,cInstrimentIDLength);
	lpInputOrder->m_operation.m_Key = GenerateOrderKey();
	lpInputOrder->m_operation.m_OrderType = OrderType::MarketOrder;
	lret.reset(lpInputOrder);
	m_SendOrderSet.insert(lpInputOrder->m_operation.m_Key);
	return lret;
}
}