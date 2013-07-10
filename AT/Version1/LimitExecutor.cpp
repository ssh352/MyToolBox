#include "LimitExecutor.h"
#include "IDriver_TD.h"
#include "ATLogger.h"
#include "EnumTranslator.h"
#include <boost\property_tree\ptree.hpp>
#include <boost\property_tree\xml_parser.hpp>
#include <sstream>

using namespace std;
namespace AT
{
	LimitExecutor::LimitExecutor( const std::string& aConfigFile )
	{
		LimitExecutorParma parma = ReadConfigFile(aConfigFile);
		InitFromParma(parma);
	}

	LimitExecutor::LimitExecutor( LimitExecutorParma parma )
	{
		InitFromParma(parma);
	}

	void LimitExecutor::InitFromParma( LimitExecutorParma parma)
	{
		m_Parma = parma;
		m_PriceType = CTP::EnumTranslator::TransOrderPriceType(m_Parma.PriceType);
	}

	LimitExecutorParma LimitExecutor::ReadConfigFile( const std::string& aConfigFile )
	{
		LimitExecutorParma lret;
		boost::property_tree::ptree lConfigPtree;
		read_xml(aConfigFile,lConfigPtree);
		m_ExecutorID = lConfigPtree.get<std::string>("ExecutorConfig.ExecutorID");
		return lret;
	}


	LimitExecutor::~LimitExecutor(void)
	{
	}

	std::string LimitExecutor::GetExecutorID()
	{
		return m_Parma.ExecutorID;
	}

	AT::Command LimitExecutor::AddExecution( ExecutorInput aExecutorInput )
	{
		if(m_ExecutionStatus.IsFinised != true)
		{
			ATLOG(L_ERROR,"Last Task not Complete");
			return InvalidCommand;
		}
		Command lOrder = BuildCommand(aExecutorInput);
		m_ExecutionStatus.AddTastVol = aExecutorInput.vol;
		m_OrderKey = lOrder.m_InputOrder.m_Key;
		m_ExecutionStatus.IsFinised = false;
		return lOrder;
	}


	AT::Command LimitExecutor::BuildCommand( ExecutorInput aNewOrder )
	{
		Command lRet;
		lRet.m_CommandType = CommandType::Input;
		InputOrder& lInputOrder = lRet.m_InputOrder;
		lInputOrder.m_OrderPriceType = m_PriceType;
		if( m_PriceType == AT::OrderPriceType::LimitPrice)
		{
			lInputOrder.m_Price =  aNewOrder.LastMarketData.m_LastPrice + m_Parma.PriceOffset * 100;
		}
		lInputOrder.m_Vol = aNewOrder.vol;
		strcpy_s(lInputOrder.InstrumentID , cInstrimentIDLength,aNewOrder.InstrumentID);
		lInputOrder.m_Key = GenerateOrderKey();
		lInputOrder.m_OpenCloseType = aNewOrder.IsOpen ;
		lInputOrder.m_BuySellType = aNewOrder.IsBuy;
		lInputOrder.m_TimeInForceCode = AT::TimeInForceType::GFD;
		lInputOrder.m_TriggerType = AT::TriggerType::Immediately;
		return lRet;
	}


	Command LimitExecutor::OnMarketDepth( const AT::MarketData& aMarketDepth )
	{
		return InvalidCommand;
	}

	Command LimitExecutor::OnRtnTrade( const AT::TradeUpdate& aTrade )
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

	Command LimitExecutor::OnRtnOrder( const AT::OrderUpdate& aOrder )
	{
		if (m_OrderKey == aOrder.m_Key)
		{
			ATLOG(L_INFO,ToString(aOrder));
			m_TheOnlyOneLimitOrder = aOrder;

			if(aOrder.m_OrderStatus == OrderStatusType::AllTraded || aOrder.m_OrderStatus == OrderStatusType::Canceled)
			{
				m_ExecutionStatus.IsFinised = true;
			}
			SetupExecutionStatus(aOrder);
		}
		return InvalidCommand;
	}



	void LimitExecutor::SetupExecutionStatus( const AT::OrderUpdate &aOrder )
	{
		m_ExecutionStatus.SuspendVol_Exechange = 0;
		m_ExecutionStatus.SuspendVol_Local = 0;
		m_ExecutionStatus.TradeVol = aOrder.m_TradedVol;
		m_ExecutionStatus.LivelVol = aOrder.m_LiveVol;
		m_ExecutionStatus.CancelVol = aOrder.m_Vol - aOrder.m_LiveVol - aOrder.m_TradedVol ;
	}

	AT::Command LimitExecutor::Abrot()
	{
		if(m_ExecutionStatus.IsFinised == true)
		{
			return InvalidCommand;
		}

		Command lret;
		lret.m_CommandType = CommandType::Cancel;
		lret.m_CancelOrder.m_Key = m_OrderKey;
		return lret;
	}

	AT::ExecutionStatus LimitExecutor::GetExecutionStatus()
	{
		return m_ExecutionStatus;
	}




}