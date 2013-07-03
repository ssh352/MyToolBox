#include "OpenMarketExecutor.h"
#include "IDriver_TD.h"
#include "../AT_Driver/ATLogger.h"
namespace AT
{


OpenMarketExecutor::OpenMarketExecutor()
{
}


OpenMarketExecutor::~OpenMarketExecutor(void)
{
}

std::string OpenMarketExecutor::GetExecutorID()
{
	return "MarketOpen";
}

boost::shared_ptr<TradeCommand> OpenMarketExecutor::AddTarget( int addTargetQuantity, bool isBuy,const AT::MarketData& aMarket )
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

	lpInputOrder->m_operation.m_OrderType = OrderType::MarketOrder;

	lret.reset(lpInputOrder);
	m_SendOrderSet.insert(lpInputOrder->m_operation.m_Key);
	m_TragetVol = addTargetQuantity;
	m_IsBuy = isBuy;
	return lret;
}

boost::shared_ptr<TradeCommand> OpenMarketExecutor::OnMarketDepth( const AT::MarketData& aMarketDepth )
{
	{
		boost::shared_ptr<TradeCommand> lret;
		lret.reset(new InvalidCommand);
		return lret;
	}
}

boost::shared_ptr<TradeCommand> OpenMarketExecutor::OnRtnTrade( const AT::TradeUpdate& apTrade )
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

boost::shared_ptr<TradeCommand> OpenMarketExecutor::OnRtnOrder( const AT::OrderUpdate& apOrder )
{
	{
		boost::shared_ptr<TradeCommand> lret;
		lret.reset(new InvalidCommand);
		return lret;
	}
}

boost::shared_ptr<TradeCommand> OpenMarketExecutor::SetupTarget( int targetQuantity,bool isBuy, const AT::MarketData& aMarket )
{
	return AddTarget(targetQuantity,isBuy,aMarket);
}

}