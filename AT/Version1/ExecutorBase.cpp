#include "ExecutorBase.h"

#include "ATLogger.h"

#include <boost\property_tree\ptree.hpp>
#include <boost\property_tree\xml_parser.hpp>

namespace AT
{

ExecutorBase::ExecutorBase( const std::string& aConfigFile )
{
	boost::property_tree::ptree lConfigPtree;
	read_xml(aConfigFile,lConfigPtree);
	m_ExecutorIDBase = lConfigPtree.get<std::string>("ExecutorConfig.ExecutorID");

	m_CommandHandle =  boost::bind(&ExecutorBase::CommandPoster,this,_1);;
}


ExecutorBase::~ExecutorBase(void)
{
}

void ExecutorBase::SetTradeReportCallback( TradeReportFun aFinishCallback )
{
	m_TradeReport = aFinishCallback;
}

void ExecutorBase::SetCommandHandler( CommandHandler aCommandHandler )
{
	m_RealCommandHandle = aCommandHandler;
}

void ExecutorBase::AddExecution( ExecutorInput aExecutorInput )
{
	if (aExecutorInput.vol == 0)
	{
		m_ExecutionStatusBase.AddTastVol = 0;
		m_ExecutionStatusBase.TradeVol = 0;//已经成交的数量
		m_ExecutionStatusBase.LivelVol = 0;	//还激活在交易所的数量
		m_ExecutionStatusBase.SuspendVol_Exechange = 0; //在交易所挂起的数量 如挂到交易所得部分单子
		m_ExecutionStatusBase.SuspendVol_Local = 0;	 //本地挂起的数量
		m_ExecutionStatusBase.CancelVol = 0;
		m_ExecutionStatusBase.IsFinised = true;
		ATLOG(L_WARN," Add Task with Vol 0 , mark Executor Finished Directly");
		return;
	}
	else
	{
		m_ExecutionStatusBase.AddTastVol  = aExecutorInput.vol;
	}

	m_InstrumentIDBase = aExecutorInput.InstrumentID;
	DoAddExecution(aExecutorInput);
}

void ExecutorBase::CommandPoster( Command aCommand )
{
	if(aCommand.m_CommandType == CommandType::Input)
	{
		m_OrderKeyBase = aCommand.m_InputOrder.m_Key;
	}
	m_RealCommandHandle(aCommand);
}

void ExecutorBase::OnMarketDepth( const AT::MarketData& aMarketDepth )
{
	if(aMarketDepth.InstrumentID == m_InstrumentIDBase)
	{
		DoOnMarketDepth(aMarketDepth);
	}
}

void ExecutorBase::Abrot()
{
	if(m_ExecutionStatusBase.IsFinised == true)
	{
		ATLOG(L_WARN," Abort a Executor when it is Finished");
		return ;
	}
	DoAbrot();
}

void ExecutorBase::OnRtnOrder( const AT::OrderUpdate& apOrder )
{
	if(apOrder.m_Key == m_OrderKeyBase)
	{
		DoOnRtnOrder(apOrder);
	}
}

void ExecutorBase::OnRtnTrade( const AT::TradeUpdate& apTrade )
{
	if(apTrade.m_Key == m_OrderKeyBase)
	{
		DoOnRtnTrade(apTrade);
	}
}

std::string ExecutorBase::GetExecutorID()
{
	return m_ExecutorIDBase;
}

AT::ExecutionStatus ExecutorBase::GetExecutionStatus()
{
	return m_ExecutionStatusBase;
}

void ExecutorBase::SetupTradeInfoBase( const AT::OrderUpdate &aOrder )
{
	m_ExecutionStatusBase.SuspendVol_Exechange = 0;
	m_ExecutionStatusBase.SuspendVol_Local = 0;
	m_ExecutionStatusBase.TradeVol = aOrder.m_TradedVol;
	m_ExecutionStatusBase.LivelVol = aOrder.m_LiveVol;
	m_ExecutionStatusBase.CancelVol = aOrder.m_Vol - aOrder.m_LiveVol - aOrder.m_TradedVol ;
}


}
