#include "OpenMarketExecutor.h"
#include "IDriver_TD.h"
#include "ATLogger.h"
#include <boost\property_tree\ptree.hpp>
#include <boost\property_tree\xml_parser.hpp>
namespace AT
{


OpenMarketExecutor::OpenMarketExecutor( const std::string& aConfig )
{
	boost::property_tree::ptree lConfigPtree;
	read_xml(aConfig,lConfigPtree);
	m_ExecutorID = lConfigPtree.get<int>("ExecutorConfig.ExecutorID");
}


OpenMarketExecutor::~OpenMarketExecutor(void)
{
}

std::string OpenMarketExecutor::GetExecutorID()
{
	return m_ExecutorID;
}
AT::Command OpenMarketExecutor::AddExecution( ExecutorInput aExecutorInput )
{
	if(aExecutorInput.vol = 0)
	{
		return InvalidCommand;
	}
	else
	{
		Command lRet =  BuildCommand( aExecutorInput);
		m_ExecutionStatus.AddTastVol += aExecutorInput.vol;
		m_OrderKey = lRet.m_InputOrder.m_Key;
		m_ExecutionStatus.IsFinised = false;
		return lRet;
	}
}


AT::Command OpenMarketExecutor::BuildCommand( ExecutorInput aNewOrder )
{
	Command lRet;
	lRet.m_CommandType = CommandType::Input;
	InputOrder& lInputOrder = lRet.m_InputOrder;

	lInputOrder.m_Price =  aNewOrder.LastMarketData.m_LastPrice;
	lInputOrder.m_Vol = aNewOrder.vol;

	strcpy_s(lInputOrder.InstrumentID , cInstrimentIDLength,aNewOrder.InstrumentID);
	lInputOrder.m_Key = GenerateOrderKey();

	lInputOrder.m_OpenCloseType = aNewOrder.IsOpen ;
	lInputOrder.m_BuySellType = aNewOrder.IsBuy;
	lInputOrder.m_OrderPriceType = AT::OrderPriceType::MarketPrice;
	lInputOrder.m_TimeInForceCode = AT::TimeInForceType::GFD;
	lInputOrder.m_TriggerType = AT::TriggerType::Immediately;
	return lRet;
}

Command OpenMarketExecutor::OnMarketDepth( const AT::MarketData& aMarketDepth )
{
	return InvalidCommand;
}

Command OpenMarketExecutor::OnRtnTrade( const AT::TradeUpdate& aTrade )
{
	if (m_OrderKey == aTrade.m_Key )
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

	return InvalidCommand;
}

Command OpenMarketExecutor::OnRtnOrder( const AT::OrderUpdate& aOrder )
{
	if (m_OrderKey == aOrder.m_Key)
	{
		ATLOG(L_INFO,ToString(aOrder));
		m_TheOnlyOneMarketOrder = aOrder;
		switch (aOrder.m_OrderStatus)
		{
		case OrderStatusType::AllTraded:
		case OrderStatusType::Canceled:
			m_ExecutionStatus.IsFinised = true;
			SetupExecutionStatus(aOrder);
			break;
		default:
			m_ExecutionStatus.IsFinised = false;
			SetupExecutionStatus(aOrder);
			break;
		}
	}
	return InvalidCommand;
}

AT::ExecutionStatus OpenMarketExecutor::GetExecutionStatus()
{
	return m_ExecutionStatus;
}

AT::Command OpenMarketExecutor::Abrot()
{

	Command lret;;
	switch (m_TheOnlyOneMarketOrder.m_OrderStatus)
	{
	case OrderStatusType::AllTraded:
	case OrderStatusType::Canceled:
		return InvalidCommand;
		break;
	default:
		lret.m_CancelOrder.m_Key = m_TheOnlyOneMarketOrder.m_Key;
		return lret;
		break;
	}
}

void OpenMarketExecutor::SetupExecutionStatus( const AT::OrderUpdate &aOrder )
{
	m_ExecutionStatus.SuspendVol_Exechange = 0;
	m_ExecutionStatus.SuspendVol_Local = 0;
	m_ExecutionStatus.TradeVol = aOrder.m_TradedVol;
	m_ExecutionStatus.LivelVol = aOrder.m_LiveVol;
	m_ExecutionStatus.CancelVol = aOrder.m_Vol - aOrder.m_LiveVol - aOrder.m_TradedVol ;
}

}