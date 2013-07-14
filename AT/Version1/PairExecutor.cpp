#include "PairExecutor.h"
#include "ExecutorFactory.h"
#include "ATLogger.h"
#include <boost\property_tree\ptree.hpp>
#include <boost\property_tree\xml_parser.hpp>
#include <boost\function.hpp>
#include <string>
namespace AT
{

	PairExecutor::PairExecutor( const std::string& aConfig )
	{
		boost::property_tree::ptree lConfigPtree;
		read_xml(aConfig,lConfigPtree);
		m_ExecutorID = lConfigPtree.get<std::string>("ExecutorConfig.ExecutorID");
		m_OpenExecutorType =  lConfigPtree.get<std::string>("ExecutorConfig.OpenExecutorType");
		m_OpenExecutorConfig =  lConfigPtree.get<std::string>("ExecutorConfig.OpenExecutorConfig");
		m_CloseExecutorType =  lConfigPtree.get<std::string>("ExecutorConfig.CloseExecutorType");
		m_CloseExecutorConfig =  lConfigPtree.get<std::string>("ExecutorConfig.CloseExecutorConfig");

	}

	PairExecutor::~PairExecutor(void)
	{
	}

	void PairExecutor::AddExecution( ExecutorInput aExecutorInput )
	{
		m_Status.AddTastVol = aExecutorInput.vol;
		m_OpenExecutor = ExecutorFactory::CreateExecutor(m_OpenExecutorType,m_OpenExecutorConfig);
		m_OpenExecutor->SetTradeReportCallback(boost::bind(&PairExecutor::ReceivOpenResult,this,_1));
		m_OpenExecutor->SetCommandHandler(m_CommandHandle);
		m_CareInstrumentID = aExecutorInput.InstrumentID;
		m_OpenExecutor->AddExecution(aExecutorInput);
		
	}

	ExecutorInput PairExecutor::BuildCloseInput( ExecutionResult aOpenResult )
	{

		ExecutorInput lret;
		lret.BuySellCode = aOpenResult.IsBuy == BuySellType::BuyOrder ? BuySellType::SellOrder: BuySellType::BuyOrder;
		lret.OpenCloseCode = OpenCloseType::CloseToday;
		lret.TriggerMarketData = m_LastMarketData;
		strcpy_s(lret.InstrumentID ,cInstrimentIDLength,aOpenResult.InstrumentID);
		lret.vol = aOpenResult.vol;

		return lret;
	}

	void PairExecutor::ReceivOpenResult( ExecutionResult aResult )
	{
		ATLOG(L_INFO,"以上为开仓成交");

		boost::shared_ptr<IExecutor> lpCloseExecutor= ExecutorFactory::CreateExecutor(m_CloseExecutorType,m_CloseExecutorConfig);
		lpCloseExecutor->SetTradeReportCallback(boost::bind(&PairExecutor::ReceiveCloseResult,this,_1));
		lpCloseExecutor->SetCommandHandler(m_CommandHandle);

		ExecutorInput lCLoseInput = BuildCloseInput(aResult);
		
		lpCloseExecutor->AddExecution(lCLoseInput);
		m_CloseExecutorGroup.push_back(lpCloseExecutor);

		m_TradeReport(aResult);
	}



	void PairExecutor::ReceiveCloseResult( ExecutionResult aResult )
	{
		ATLOG(L_INFO,"以上为平仓成交");
		m_TradeReport(aResult);
	}

	void PairExecutor::Abrot()
	{
		 m_OpenExecutor->Abrot();
		for (auto clostExecutorPtr:m_CloseExecutorGroup)
		{
			clostExecutorPtr->Abrot();
		}
	}

	void PairExecutor::OnMarketDepth( const AT::MarketData& aMarketDepth )
	{

		if(m_CareInstrumentID != aMarketDepth.InstrumentID)
		{
			return ;
		}
		m_LastMarketData  = aMarketDepth;
		m_OpenExecutor->OnMarketDepth(aMarketDepth);
		for (auto clostExecutorPtr:m_CloseExecutorGroup)
		{
			clostExecutorPtr->OnMarketDepth(aMarketDepth);
		}
	}

	void PairExecutor::OnRtnOrder( const AT::OrderUpdate& apOrder )
	{
		m_OpenExecutor->OnRtnOrder(apOrder);
		for (auto clostExecutorPtr:m_CloseExecutorGroup)
		{
			clostExecutorPtr->OnRtnOrder(apOrder);
		}

		CheckFinished();

	}

	void PairExecutor::CheckFinished()
	{
		bool lFinised = true;
		lFinised = lFinised && m_OpenExecutor->GetExecutionStatus().IsFinised;
		for (auto clostExecutorPtr:m_CloseExecutorGroup)
		{
			lFinised = lFinised && clostExecutorPtr->GetExecutionStatus().IsFinised;
		}
		m_Status.IsFinised = lFinised;
	}

	void PairExecutor::OnRtnTrade( const AT::TradeUpdate& apTrade )
	{
		m_OpenExecutor->OnRtnTrade(apTrade);
		for (auto clostExecutorPtr:m_CloseExecutorGroup)
		{
			clostExecutorPtr->OnRtnTrade(apTrade);
		}
	}

	AT::ExecutionStatus PairExecutor::GetExecutionStatus()
	{
		return m_Status;
	}

	std::string PairExecutor::GetExecutorID()
	{
		return m_ExecutorID;
	}


}

