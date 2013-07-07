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
	lRet.m_InputOrder.m_Price =  aNewOrder.LastMarketData.m_LastPrice;

	lRet.m_InputOrder.m_OpenCloseType = aNewOrder.IsOpen ;
	lRet.m_InputOrder.m_BuySellType = aNewOrder.IsBuy;
	lRet.m_InputOrder.m_Vol = aNewOrder.vol;
	strcpy_s(lRet.m_InputOrder.InstrumentID , cInstrimentIDLength,aNewOrder.InstrumentID);
	lRet.m_InputOrder.m_Key = GenerateOrderKey();
	lRet.m_InputOrder.m_OrderPriceType = OrderPriceType::MarketOrder;
	return lRet;
}

Command OpenMarketExecutor::OnMarketDepth( const AT::MarketData& aMarketDepth )
{
	return InvalidCommand;
}

Command OpenMarketExecutor::OnRtnTrade( const AT::TradeUpdate& apTrade )
{
	if (m_SendOrderSet.find(apTrade.m_Key ) != m_SendOrderSet.end())
	{
		m_ExecutionStatus.LivelVol -= apTrade.m_TradeVol;
		m_ExecutionStatus.TradeVol += apTrade.m_TradeVol;
		m_ExecutionStatus.IsFinised = m_ExecutionStatus.LivelVol == 0;

		ExecutionResult lResult ;
		strcpy_s(lResult.InstrumentID,cInstrimentIDLength,apTrade.InstrumentID);
		lResult.IsBuy = apTrade.m_BuySellType;
		lResult.IsOpen = apTrade.m_OpenCloseType;
		lResult.Price = apTrade.m_TradePrice;
		lResult.vol = apTrade.m_TradeVol;

		m_TradeReport(lResult);	
		ATLOG(L_INFO,apTrade.ToString());
		
	}

	return InvalidCommand;
}

Command OpenMarketExecutor::OnRtnOrder( const AT::OrderUpdate& apOrder )
{
	if (m_SendOrderSet.find(apOrder.m_Key ) != m_SendOrderSet.end())
	{

		ATLOG(L_INFO,apOrder.ToString());
	}
	return InvalidCommand;
}

AT::ExecutionStatus OpenMarketExecutor::GetExecutionStatus()
{

}

AT::Command OpenMarketExecutor::Abrot()
{

}




}