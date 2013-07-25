#include "FollowExecutor.h"
#include "IDriver_TD.h"
#include "ATLogger.h"
#include <boost\property_tree\ptree.hpp>
#include <boost\property_tree\xml_parser.hpp>
namespace AT
{

FollowExecutor::FollowExecutor( const boost::property_tree::ptree& aConfigPtee )
	:ExecutorBase(aConfigPtee)
	,m_Status(FollowStatus::BeforeStart)
{

}


FollowExecutor::~FollowExecutor(void)
{
}



AT::Command FollowExecutor::BuildCommand( int vol)
{
	Command lRet;
	lRet.m_CommandType = CommandType::Input;
	InputOrder& lInputOrder = lRet.m_InputOrder;
	lInputOrder.m_Vol = vol;
	strcpy_s(lInputOrder.InstrumentID , cInstrimentIDLength,m_InstrumentIDBase.c_str());
	lInputOrder.m_Key = GenerateOrderKey();
	lInputOrder.m_OpenCloseType =  m_OpenClose;
	lInputOrder.m_BuySellType = m_BuySell ;
	lInputOrder.m_OrderPriceType = AT::OrderPriceType::LastPrice;
	lInputOrder.m_TimeInForceCode = AT::TimeInForceType::IOC;
	lInputOrder.m_TriggerType = AT::TriggerType::Immediately;
	return lRet;
}


void FollowExecutor::DoOnRtnTrade( const AT::TradeUpdate& aTrade )
{
	if (m_OrderKeyBase == aTrade.m_Key )
	{
		ExecutionResult lResult ;
		strcpy_s(lResult.InstrumentID,cInstrimentIDLength,aTrade.InstrumentID);
		lResult.IsBuy = aTrade.m_BuySellType;
		lResult.IsOpen = aTrade.m_OpenCloseType;
		lResult.Price = aTrade.m_TradePrice;
		lResult.vol = aTrade.m_TradeVol;

		m_TradeReport(lResult);	
		ATLOG(L_INFO,ToString(aTrade));
	}
}

void FollowExecutor::SetupTradeInfo( const AT::OrderUpdate &aOrder )
{
	m_ExecutionStatusBase.TradeVol += aOrder.m_TradedVol;
	m_ExecutionStatusBase.CancelVol = m_ExecutionStatusBase.AddTastVol - m_ExecutionStatusBase.TradeVol;
}

void FollowExecutor::DoAddExecution( ExecutorInput aExecutorInput )
{
	m_BuySell = aExecutorInput.BuySellCode;
	m_OpenClose = aExecutorInput.OpenCloseCode;
	Command lRet =  BuildCommand( aExecutorInput.vol);
	m_ExecutionStatusBase.IsFinised = false;
	m_Status = FollowStatus::Running;
	m_CommandHandle(lRet);
}

void FollowExecutor::DoAbrot()
{

	switch (m_Status)
	{
	case AT::FollowExecutor::FollowStatus::BeforeStart:
		return ;
		break;
	case AT::FollowExecutor::FollowStatus::Running:
		{
			Command lret;
			lret.m_CommandType = CommandType::Cancel;
			lret.m_CancelOrder.m_Key = m_OrderKeyBase;
			m_CommandHandle( lret);
			m_Status = FollowStatus::PendingCancel;
		}
		break;
	case AT::FollowExecutor::FollowStatus::PendingCancel:
		return;
		break;
	case AT::FollowExecutor::FollowStatus::Finished:
		return;
		break;
	default:
		break;
	}
}

void FollowExecutor::DoOnMarketDepth( const AT::MarketData& aMarketDepth )
{
	return ;
}

void FollowExecutor::DoOnRtnOrder( const AT::OrderUpdate& aOrder )
{

		ATLOG(L_INFO,ToString(aOrder));
		SetupTradeInfo(aOrder);

		if(aOrder.m_OrderStatus == OrderStatusType::AllTraded)
		{
			m_ExecutionStatusBase.IsFinised = true;
		}
		else if( aOrder.m_OrderStatus == OrderStatusType::Canceled && m_Status == FollowStatus::PendingCancel)
		{
			m_ExecutionStatusBase.IsFinised = true;
			m_Status = FollowStatus::Finished;
		}
		else
		{
			//这里需要检查，假如一个IOC单子一下有多个对手单来与其成交 是否只来一个报单回报
			Command lretCommand = BuildCommand(m_ExecutionStatusBase.CancelVol);
			//m_OrderKeyBase = lretCommand.m_InputOrder.m_Key;  // the order key is save by ExecutorBase
			m_CommandHandle( lretCommand);
		}
}

}