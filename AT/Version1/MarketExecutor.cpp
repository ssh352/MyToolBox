#include "MarketExecutor.h"
#include "IDriver_TD.h"
#include "ATLogger.h"
namespace AT
{

MarketExecutor::MarketExecutor( const std::string& aConfig )
	:ExecutorBase(aConfig)
{

}

MarketExecutor::~MarketExecutor(void)
{
}

Command MarketExecutor::BuildCommand( ExecutorInput aNewOrder )
{
	Command lRet;
	lRet.m_CommandType = CommandType::Input;
	InputOrder& lInputOrder = lRet.m_InputOrder;

	lInputOrder.m_Price =  0/*aNewOrder.TriggerMarketData.m_LastPrice*/;
	lInputOrder.m_Vol = aNewOrder.vol;

	strcpy_s(lInputOrder.InstrumentID , cInstrimentIDLength,aNewOrder.InstrumentID);
	lInputOrder.m_Key = GenerateOrderKey();

	lInputOrder.m_OpenCloseType = aNewOrder.OpenCloseCode ;
	lInputOrder.m_BuySellType = aNewOrder.BuySellCode;
	lInputOrder.m_OrderPriceType = AT::OrderPriceType::MarketPrice;
	lInputOrder.m_TimeInForceCode = AT::TimeInForceType::IOC;
	lInputOrder.m_TriggerType = AT::TriggerType::Immediately;
	return lRet;
}
void MarketExecutor::DoAddExecution( ExecutorInput aExecutorInput )
{
	Command lRet =  BuildCommand( aExecutorInput);
	m_CommandHandle(lRet);
}

void MarketExecutor::DoOnMarketDepth( const AT::MarketData& aMarketDepth )
{
	return;
}

void MarketExecutor::DoAbrot()
{
	Command lret;
	lret.m_CommandType = CommandType::Cancel;
	lret.m_CancelOrder.m_Key = m_OrderKeyBase;
	m_CommandHandle(lret);
}

void MarketExecutor::DoOnRtnOrder( const AT::OrderUpdate& aOrder )
{
	ATLOG(L_INFO,ToString(aOrder));
	if(aOrder.m_OrderStatus == OrderStatusType::AllTraded || aOrder.m_OrderStatus == OrderStatusType::Canceled)
	{
		m_ExecutionStatusBase.IsFinised = true;
	}
	SetupTradeInfoBase(aOrder);
	return ;
}

void MarketExecutor::DoOnRtnTrade( const AT::TradeUpdate& aTrade )
{
	ExecutionResult lResult ;
	strcpy_s(lResult.InstrumentID,cInstrimentIDLength,aTrade.InstrumentID);
	lResult.IsBuy = aTrade.m_BuySellType;
	lResult.IsOpen = aTrade.m_OpenCloseType;
	lResult.Price = aTrade.m_TradePrice;
	lResult.vol = aTrade.m_TradeVol;

	m_TradeReport(lResult);	
	ATLOG(L_INFO,ToString(aTrade));
	return;
}

}