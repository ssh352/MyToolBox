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
		:ExecutorBase(aConfigFile)
	{
		LimitExecutorParma parma = ReadConfigFile(aConfigFile);
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
		m_ExecutorIDBase = lConfigPtree.get<std::string>("ExecutorConfig.ExecutorID");
		lret.ExecutorID = m_ExecutorIDBase;
		lret.PriceType = lConfigPtree.get<char>("ExecutorConfig.PriceType");
		lret.PriceOffset = lConfigPtree.get<int>("ExecutorConfig.PriceOffSet");
		return lret;
	}


	LimitExecutor::~LimitExecutor(void)
	{
	}

	AT::Command LimitExecutor::BuildCommand( ExecutorInput aNewOrder )
	{
		Command lRet;
		lRet.m_CommandType = CommandType::Input;
		InputOrder& lInputOrder = lRet.m_InputOrder;
		lInputOrder.m_OrderPriceType = m_PriceType;
		if( m_PriceType == AT::OrderPriceType::LimitPrice)
		{
			lInputOrder.m_Price =  aNewOrder.TriggerMarketData.m_LastPrice + m_Parma.PriceOffset * 1000;
		}
		lInputOrder.m_Vol = aNewOrder.vol;
		strcpy_s(lInputOrder.InstrumentID , cInstrimentIDLength,aNewOrder.InstrumentID);
		lInputOrder.m_Key = GenerateOrderKey();
		lInputOrder.m_OpenCloseType = aNewOrder.OpenCloseCode ;
		lInputOrder.m_BuySellType = aNewOrder.BuySellCode;
		lInputOrder.m_TimeInForceCode = AT::TimeInForceType::GFD;
		lInputOrder.m_TriggerType = AT::TriggerType::Immediately;
		return lRet;
	}

	void LimitExecutor::DoAbrot()
	{
		Command lret;
		lret.m_CommandType = CommandType::Cancel;
		lret.m_CancelOrder.m_Key = m_OrderKeyBase;
		m_CommandHandle(lret);
	}


	void LimitExecutor::DoAddExecution( ExecutorInput aExecutorInput )
	{
		Command lOrder = BuildCommand(aExecutorInput);
		m_ExecutionStatusBase.IsFinised = false;
		m_CommandHandle(lOrder);
	}

	void LimitExecutor::DoOnMarketDepth( const AT::MarketData& aMarketDepth )
	{
		return ;
	}

	void LimitExecutor::DoOnRtnOrder( const AT::OrderUpdate& aOrder )
	{
		ATLOG(L_INFO,ToString(aOrder));

		if(aOrder.m_OrderStatus == OrderStatusType::AllTraded || aOrder.m_OrderStatus == OrderStatusType::Canceled)
		{
			m_ExecutionStatusBase.IsFinised = true;
		}
		SetupTradeInfoBase(aOrder);
	}

	void LimitExecutor::DoOnRtnTrade( const AT::TradeUpdate& aTrade )
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