#include "MarketExecutor.h"
#include "IDriver_TD.h"
#include "ATLogger.h"
#include <boost\property_tree\ptree.hpp>
#include <boost\property_tree\xml_parser.hpp>
namespace AT
{


MarketExecutor::MarketExecutor( const std::string& aConfig )
{
	boost::property_tree::ptree lConfigPtree;
	read_xml(aConfig,lConfigPtree);
	m_ExecutorID = lConfigPtree.get<std::string>("ExecutorConfig.ExecutorID");
}


MarketExecutor::~MarketExecutor(void)
{
}

std::string MarketExecutor::GetExecutorID()
{
	return m_ExecutorID;
}
void MarketExecutor::AddExecution( ExecutorInput aExecutorInput )
{
	if(aExecutorInput.vol = 0)
	{
		return ;
	}
	else
	{
		if(m_ExecutionStatus.IsFinised != true)
		{
			ATLOG(L_ERROR,"Last Task not Complete");
			return ;
		}
		Command lRet =  BuildCommand( aExecutorInput);
		m_ExecutionStatus.AddTastVol = aExecutorInput.vol;
		m_OrderKey = lRet.m_InputOrder.m_Key;
		m_ExecutionStatus.IsFinised = false;
		m_CommandHandle(lRet);
	}
}


Command MarketExecutor::BuildCommand( ExecutorInput aNewOrder )
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
	lInputOrder.m_OrderPriceType = AT::OrderPriceType::MarketPrice;
	lInputOrder.m_TimeInForceCode = AT::TimeInForceType::GFD;
	lInputOrder.m_TriggerType = AT::TriggerType::Immediately;
	return lRet;
}

void MarketExecutor::OnMarketDepth( const AT::MarketData& aMarketDepth )
{
	return ;
}

void MarketExecutor::OnRtnTrade( const AT::TradeUpdate& aTrade )
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

	return ;
}

void MarketExecutor::OnRtnOrder( const AT::OrderUpdate& aOrder )
{
	if (m_OrderKey == aOrder.m_Key)
	{
		ATLOG(L_INFO,ToString(aOrder));
		m_TheOnlyOneMarketOrder = aOrder;

		if(aOrder.m_OrderStatus == OrderStatusType::AllTraded || aOrder.m_OrderStatus == OrderStatusType::Canceled)
		{
			m_ExecutionStatus.IsFinised = true;
		}
		SetupExecutionStatus(aOrder);
	}
	return ;
}

AT::ExecutionStatus MarketExecutor::GetExecutionStatus()
{
	return m_ExecutionStatus;
}

void MarketExecutor::Abrot()
{
	if(m_ExecutionStatus.IsFinised == true)
	{
		return ;
	}

	Command lret;
	lret.m_CommandType = CommandType::Cancel;
	lret.m_CancelOrder.m_Key = m_TheOnlyOneMarketOrder.m_Key;
	m_CommandHandle(lret);

}

void MarketExecutor::SetupExecutionStatus( const AT::OrderUpdate &aOrder )
{
	m_ExecutionStatus.SuspendVol_Exechange = 0;
	m_ExecutionStatus.SuspendVol_Local = 0;
	m_ExecutionStatus.TradeVol = aOrder.m_TradedVol;
	m_ExecutionStatus.LivelVol = aOrder.m_LiveVol;
	m_ExecutionStatus.CancelVol = aOrder.m_Vol - aOrder.m_LiveVol - aOrder.m_TradedVol ;
}

}