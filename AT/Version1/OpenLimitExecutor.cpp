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
	lret.reset(lpInputOrder);
	m_SendOrderSet.insert(lpInputOrder->m_operation.m_Key);
	m_TragetVol = addTargetQuantity;
	m_IsBuy = isBuy;
	return lret;
}

boost::shared_ptr<TradeCommand> OpenLimitExecutor::OnMarketDepth( const AT::MarketData& aMarketDepth )
{
	m_LastMarket = aMarketDepth;
	if(m_LastMarket.m_UpdateTime - m_StartTime > boost::posix_time::seconds(m_OrderValidTime))
	{
		//cancel order
		m_FinishehNotfiy(0,0,m_IsBuy,true);	
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

}