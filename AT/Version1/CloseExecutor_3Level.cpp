#include "CloseExecutor_3Level.h"

namespace AT
{


CloseExecutor_3Level::~CloseExecutor_3Level(void)
{
}

CloseExecutor_3Level::CloseExecutor_3Level( CloseSetting_3Level aSetting)
	:m_Setting(aSetting)
	,m_CurrentLevel(0)
{

}

boost::shared_ptr<TradeCommand> CloseExecutor_3Level::SetupTarget( int targetQuantity,const AT::MarketData& aMarket )
{
	m_CurrentLevel = 0;
	m_StartPrice = aMarket.m_LastPrice;
	m_TargetVol = targetQuantity;

	boost::shared_ptr<TradeCommand> lret;
	lret.reset(new InvalidCommand);
	return lret;
}

boost::shared_ptr<TradeCommand> CloseExecutor_3Level::AddTarget( int addTargetQuantity, const AT::MarketData& aMarket )
{
	m_TargetVol += addTargetQuantity;

	boost::shared_ptr<TradeCommand> lret;
	lret.reset(new InvalidCommand);
	return lret;
}

boost::shared_ptr<TradeCommand> CloseExecutor_3Level::OnMarketDepth( const AT::MarketData& aMarketDepth )
{
	if(m_TargetVol > 0)
	{
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

	lpInputOrder->m_operation.m_Vol = m_TargetVol;
	m_ActiveOrderVol += m_TargetVol;
	m_TargetVol = 0;
	strncpy_s(lpInputOrder->m_operation.InstrumentID , cInstrimentIDLength,aMarket.InstrumentID,cInstrimentIDLength);
	//.... todo more
	lret.reset(lpInputOrder);
	return lret;

}

boost::shared_ptr<TradeCommand> CloseExecutor_3Level::OnRtnOrder( const AT::OrderUpdate& apOrder )
{
	boost::shared_ptr<TradeCommand> lret;
	lret.reset(new InvalidCommand);
	return lret;
}

boost::shared_ptr<TradeCommand> CloseExecutor_3Level::OnRtnTrade( const AT::TradeUpdate& apTrade )
{
	m_ActiveOrderVol -= apTrade.m_TradeVol;
	boost::shared_ptr<TradeCommand> lret;
	lret.reset(new InvalidCommand);
	return lret;
}

}