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
		m_SendOrderSet.insert(lRet.m_InputOrder.m_Key);
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
	if (m_SendOrderSet.find(aTrade.m_Key ) != m_SendOrderSet.end())
	{
		m_ExecutionStatus.LivelVol -= aTrade.m_TradeVol;
		m_ExecutionStatus.TradeVol += aTrade.m_TradeVol;
		m_ExecutionStatus.IsFinised = m_ExecutionStatus.LivelVol == 0;

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
	if (m_SendOrderSet.find(aOrder.m_Key ) != m_SendOrderSet.end())
	{

		ATLOG(L_INFO,ToString(aOrder));
	}
	return InvalidCommand;
}

AT::ExecutionStatus OpenMarketExecutor::GetExecutionStatus()
{
	ExecutionStatus lret;
	return lret;
}

AT::Command OpenMarketExecutor::Abrot()
{
	Command lret;
	return lret;
}




}