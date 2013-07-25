#include "StopLossExecutor.h"
#include "IDriver_TD.h"
#include "ATLogger.h"
#include <boost\property_tree\ptree.hpp>
#include <boost\property_tree\xml_parser.hpp>
namespace AT
{


StopLossExecutor::StopLossExecutor( const boost::property_tree::ptree& aConfigPtee )
	:ExecutorBase(aConfigPtee)
{
	m_StopLossOffset = aConfigPtee.get<int>("ExecutorConfig.StopOffset");
}


StopLossExecutor::~StopLossExecutor(void)
{
}

void StopLossExecutor::DoAddExecution( ExecutorInput aExecutorInput )
{
	Command lRet =  BuildCommand( aExecutorInput);
	m_ExecutionStatusBase.AddTastVol = aExecutorInput.vol;
	m_ExecutionStatusBase.IsFinised = false;
	m_CommandHandle(lRet);
}

Command StopLossExecutor::BuildCommand( ExecutorInput aNewOrder )
{
	Command lRet;
	lRet.m_CommandType = CommandType::Input;
	InputOrder& lInputOrder = lRet.m_InputOrder;

	lInputOrder.m_Price =  aNewOrder.TriggerMarketData.m_LastPrice;
	lInputOrder.m_Vol = aNewOrder.vol;

	strcpy_s(lInputOrder.InstrumentID , cInstrimentIDLength,aNewOrder.InstrumentID);
	lInputOrder.m_Key = GenerateOrderKey();

	lInputOrder.m_OpenCloseType = aNewOrder.OpenCloseCode ;
	lInputOrder.m_BuySellType = aNewOrder.BuySellCode;
	lInputOrder.m_OrderPriceType = AT::OrderPriceType::LimitPrice;
	lInputOrder.m_TimeInForceCode = AT::TimeInForceType::GFD;
	lInputOrder.m_TriggerType = AT::TriggerType::Touch;

	int StopLossPrice = aNewOrder.TriggerMarketData.m_LastPrice ;
	if(aNewOrder.BuySellCode == BuySellType::BuyOrder)
	{
	StopLossPrice += m_StopLossOffset;
	}
	else
	{
	StopLossPrice -= m_StopLossOffset;
	}

	lInputOrder.m_TriggerPrice = StopLossPrice;

	/*lInputOrder.m_Vol = aNewOrder.vol;

	strcpy_s(lInputOrder.InstrumentID , cInstrimentIDLength,aNewOrder.InstrumentID);
	lInputOrder.m_Key = GenerateOrderKey();

	lInputOrder.m_OpenCloseType = aNewOrder.OpenCloseCode ;
	lInputOrder.m_BuySellType = aNewOrder.BuySellCode;
	lInputOrder.m_OrderPriceType = AT::OrderPriceType::LimitPrice;
	lInputOrder.m_TimeInForceCode = AT::TimeInForceType::GFD;
	lInputOrder.m_TriggerType = AT::TriggerType::Immediately;

	int StopLossPrice = aNewOrder.TriggerMarketData.m_LastPrice ;
	if(aNewOrder.BuySellCode == BuySellType::BuyOrder)
	{
		StopLossPrice += m_StopLossOffset;
	}
	else
	{
		StopLossPrice -= m_StopLossOffset;
	}


	lInputOrder.m_Price = StopLossPrice;*/

	return lRet;
}

void StopLossExecutor::DoOnMarketDepth( const AT::MarketData& aMarketDepth )
{
	return ;
}

void StopLossExecutor::DoOnRtnTrade( const AT::TradeUpdate& aTrade )
{

	ExecutionResult lResult ;
	strcpy_s(lResult.InstrumentID,cInstrimentIDLength,aTrade.InstrumentID);
	lResult.IsBuy = aTrade.m_BuySellType;
	lResult.IsOpen = aTrade.m_OpenCloseType;
	lResult.Price = aTrade.m_TradePrice;
	lResult.vol = aTrade.m_TradeVol;

	m_TradeReport(lResult);	
	ATLOG(L_INFO,ToString(aTrade));
	
	return ;
}

void StopLossExecutor::DoOnRtnOrder( const AT::OrderUpdate& aOrder )
{
	
	ATLOG(L_INFO,ToString(aOrder));
	if(aOrder.m_OrderStatus == OrderStatusType::AllTraded || aOrder.m_OrderStatus == OrderStatusType::Canceled)
	{
		SetupTradeInfoBase(aOrder);
		m_ExecutionStatusBase.IsFinised = true;
	}
	return ;
}
void StopLossExecutor::DoAbrot()
{
	Command lret;
	lret.m_CommandType = CommandType::Cancel;
	lret.m_CancelOrder.m_Key = m_OrderKeyBase;
	m_CommandHandle(lret);
}

}