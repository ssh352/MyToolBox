#include "FollowExecutor.h"
#include "IDriver_TD.h"
#include "ATLogger.h"
#include <boost\property_tree\ptree.hpp>
#include <boost\property_tree\xml_parser.hpp>
namespace AT
{


FollowExecutor::FollowExecutor( const std::string& aConfig )
	:m_IsAbrot(false)
{
	boost::property_tree::ptree lConfigPtree;
	read_xml(aConfig,lConfigPtree);
	m_ExecutorID = lConfigPtree.get<std::string>("ExecutorConfig.ExecutorID");
}


FollowExecutor::~FollowExecutor(void)
{
}

std::string FollowExecutor::GetExecutorID()
{
	return m_ExecutorID;
}
void FollowExecutor::AddExecution( ExecutorInput aExecutorInput )
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

		m_BuySell = aExecutorInput.IsBuy;
		m_OpenClose = aExecutorInput.IsOpen;
		m_InstrumentID = aExecutorInput.InstrumentID;
		Command lRet =  BuildCommand( aExecutorInput.vol);
		m_ExecutionStatus.AddTastVol = aExecutorInput.vol;
		m_OrderKey = lRet.m_InputOrder.m_Key;
		m_ExecutionStatus.IsFinised = false;
		m_IsAbrot  = false;
		m_CommandHandle(lRet);
	}
}


AT::Command FollowExecutor::BuildCommand( int vol)
{
	Command lRet;
	lRet.m_CommandType = CommandType::Input;
	InputOrder& lInputOrder = lRet.m_InputOrder;

	lInputOrder.m_Vol = vol;

	strcpy_s(lInputOrder.InstrumentID , cInstrimentIDLength,m_InstrumentID.c_str());
	lInputOrder.m_Key = GenerateOrderKey();

	lInputOrder.m_OpenCloseType =  m_OpenClose;
	lInputOrder.m_BuySellType = m_BuySell ;
	lInputOrder.m_OrderPriceType = AT::OrderPriceType::LastPrice;
	lInputOrder.m_TimeInForceCode = AT::TimeInForceType::IOC;
	lInputOrder.m_TriggerType = AT::TriggerType::Immediately;
	return lRet;
}

void FollowExecutor::OnMarketDepth( const AT::MarketData& aMarketDepth )
{
	
}

void FollowExecutor::OnRtnTrade( const AT::TradeUpdate& aTrade )
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
}

void FollowExecutor::OnRtnOrder( const AT::OrderUpdate& aOrder )
{
	if (m_OrderKey == aOrder.m_Key)
	{
		ATLOG(L_INFO,ToString(aOrder));
		m_TheOnlyOneMarketOrder = aOrder;


		SetupExecutionStatus(aOrder);

		if(aOrder.m_OrderStatus == OrderStatusType::AllTraded)
		{
			m_ExecutionStatus.IsFinised = true;
		}
		else if( aOrder.m_OrderStatus == OrderStatusType::Canceled && m_IsAbrot)
		{
			m_ExecutionStatus.IsFinised = true;
		}
		else
		{
			//这里需要检查，假如一个IOC单子一下有多个对手单来与其成交 是否只来一个报单回报
			Command lretCommand = BuildCommand(m_ExecutionStatus.CancelVol);
			m_OrderKey = lretCommand.m_InputOrder.m_Key;
			m_CommandHandle( lretCommand);
		}
		
	}
}

AT::ExecutionStatus FollowExecutor::GetExecutionStatus()
{
	return m_ExecutionStatus;
}

void FollowExecutor::Abrot()
{

	if(m_ExecutionStatus.IsFinised == true)
	{
		return ;
	}

	m_IsAbrot = true;
	Command lret;
	lret.m_CommandType = CommandType::Cancel;
	lret.m_CancelOrder.m_Key = m_TheOnlyOneMarketOrder.m_Key;
	m_CommandHandle( lret);
}

void FollowExecutor::SetupExecutionStatus( const AT::OrderUpdate &aOrder )
{

	m_ExecutionStatus.TradeVol += aOrder.m_TradedVol;
	m_ExecutionStatus.CancelVol = m_ExecutionStatus.AddTastVol - m_ExecutionStatus.TradeVol;
}

}