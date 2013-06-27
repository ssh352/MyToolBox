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
{
	m_TargetVol = 0;
	m_ActiveOrderVol = 0;
}

boost::shared_ptr<TradeCommand> CloseExecutor_3Level::SetupTarget( int targetQuantity, bool isBuy, const AT::MarketData& aMarket )
{
	if(targetQuantity == 0 )
	{
		boost::shared_ptr<TradeCommand> lret;
		lret.reset(new InvalidCommand);
		return lret;
	}

	m_CurrentLevel = 0;
	m_StartPrice = aMarket.m_LastPrice;
	m_TargetVol = targetQuantity;
	m_IsBuy = isBuy;

	boost::shared_ptr<TradeCommand> lret;
	lret.reset(new InvalidCommand);
	return lret;
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

	boost::shared_ptr<TradeCommand> lret;
	lret.reset(new InvalidCommand);
	return lret;
}

boost::shared_ptr<TradeCommand> CloseExecutor_3Level::OnMarketDepth( const AT::MarketData& aMarketDepth )
{
	if(m_TargetVol > 0)
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
		switch (m_CurrentLevel)
		{
		case 0:
			if(lPriceDiffStart < m_Setting.QuitLevel_0)
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
	InputCommand* lpInputOrder = new InputCommand;

	lpInputOrder->m_operation.m_Price = aMarket.m_LastPrice;
	lpInputOrder->m_operation.m_OpenCloseType = OpenCloseType::CloseTodayOrder;
	lpInputOrder->m_operation.m_BuySellType = m_IsBuy? BuySellType::BuyOrder : BuySellType::SellOrder;
	lpInputOrder->m_operation.m_Vol = m_TargetVol;
	m_ActiveOrderVol += m_TargetVol;
	m_TargetVol = 0;
	m_CurrentLevel = 0;
	strncpy_s(lpInputOrder->m_operation.InstrumentID , cInstrimentIDLength,aMarket.InstrumentID,cInstrimentIDLength);
	lpInputOrder->m_operation.m_Key = GenerateOrderKey();
	lpInputOrder->m_operation.m_OrderType = OrderType::MarketOrder;
	lret.reset(lpInputOrder);
	m_SendOrderSet.insert(lpInputOrder->m_operation.m_Key);
	return lret;

}

boost::shared_ptr<TradeCommand> CloseExecutor_3Level::OnRtnOrder( const AT::OrderUpdate& apOrder )
{
	boost::shared_ptr<TradeCommand> lret;
	lret.reset(new InvalidCommand);

	if(m_SendOrderSet.find(apOrder.m_Key) == m_SendOrderSet.end())
	{
		return lret;
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

	m_ActiveOrderVol -= apTrade.m_TradeVol;

	bool isFinishe = m_ActiveOrderVol == 0 ;
	m_FinishehNotfiy(apTrade.m_TradePrice,apTrade.m_TradeVol,m_IsBuy,isFinishe);

	return lret;
}

std::string CloseExecutor_3Level::GetExecutorID()
{
	return "Level3Quit";
}

}