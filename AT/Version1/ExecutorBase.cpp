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
	m_ExecutorID = lConfigPtree.get<std::string>("ExecutorConfig.ExecutorID");

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
		m_ExecutionStatus.AddTastVol = 0;
		m_ExecutionStatus.TradeVol = 0;//已经成交的数量
		m_ExecutionStatus.LivelVol = 0;	//还激活在交易所的数量
		m_ExecutionStatus.SuspendVol_Exechange = 0; //在交易所挂起的数量 如挂到交易所得部分单子
		m_ExecutionStatus.SuspendVol_Local = 0;	 //本地挂起的数量
		m_ExecutionStatus.CancelVol = 0;
		m_ExecutionStatus.IsFinised = true;
		ATLOG(L_WARN," Add Task with Vol 0 , mark Executor Finished Directly");
		return;
	}

	m_InstrumentID = aExecutorInput.InstrumentID;
	DoAddExecution(aExecutorInput);
}

void ExecutorBase::CommandPoster( Command aCommand )
{
	if(aCommand.m_CommandType == CommandType::Input)
	{
		m_OrderKey = aCommand.m_InputOrder.m_Key;
	}
	m_RealCommandHandle(aCommand);
}

void ExecutorBase::OnMarketDepth( const AT::MarketData& aMarketDepth )
{
	if(aMarketDepth.InstrumentID == m_InstrumentID)
	{
		DoOnMarketDepth(aMarketDepth);
	}
}

void ExecutorBase::Abrot()
{
	if(m_ExecutionStatus.IsFinised == true)
	{
		ATLOG(L_WARN," Abort a Executor when it is Finished");
		return ;
	}
	DoAbrot();
}

void ExecutorBase::OnRtnOrder( const AT::OrderUpdate& apOrder )
{
	if(apOrder.m_Key == m_OrderKey)
	{
		DoOnRtnOrder(apOrder);
	}
}

void ExecutorBase::OnRtnTrade( const AT::TradeUpdate& apTrade )
{
	if(apTrade.m_Key == m_OrderKey)
	{
		DoOnRtnTrade(apTrade);
	}
}

std::string ExecutorBase::GetExecutorID()
{
	return m_ExecutorID;
}

AT::ExecutionStatus ExecutorBase::GetExecutionStatus()
{
	return m_ExecutionStatus;
}

void ExecutorBase::SetupExecutionStatus( const AT::OrderUpdate &aOrder )
{
	m_ExecutionStatus.SuspendVol_Exechange = 0;
	m_ExecutionStatus.SuspendVol_Local = 0;
	m_ExecutionStatus.TradeVol = aOrder.m_TradedVol;
	m_ExecutionStatus.LivelVol = aOrder.m_LiveVol;
	m_ExecutionStatus.CancelVol = aOrder.m_Vol - aOrder.m_LiveVol - aOrder.m_TradedVol ;
}


}
