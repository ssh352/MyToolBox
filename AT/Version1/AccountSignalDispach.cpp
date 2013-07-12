#include "AccountSignalDispach.h"
#include "ATLogger.h"
#include "ExecutorContianer.h"
#include <boost\property_tree\ptree.hpp>
#include <boost\property_tree\xml_parser.hpp>
namespace AT
{


AccountSignalDispach::AccountSignalDispach( const std::string& aConfigFile,ExecutorContianer* apExecutorContainer,int AccountVol )
	:m_Vol(AccountVol)
	,m_pExecutorContianer(apExecutorContainer)
{
	LoadExecuteInfoFromFile(aConfigFile);
}


AccountSignalDispach::~AccountSignalDispach(void)
{

}

void AccountSignalDispach::HandleSignal( Signal aSignal )
{
	if(m_Signal_Executor_Map.find(aSignal.m_ID) == m_Signal_Executor_Map.end())
	{
		ATLOG(L_WARN,"No Handle Signal, because did not find the Executor in the map");
		return;
	}
	const SignalExecutInfo& lExecutInfo =  m_Signal_Executor_Map[aSignal.m_ID];
	const std::string& lExectorID = lExecutInfo.ExecutorID;
	boost::shared_ptr<IExecutor> lpExecutor = m_pExecutorContianer->GetExecutorByID(lExectorID);


	ExecutorInput lExecutorInput = BuildExecutorInput(lExecutInfo,aSignal.m_TriggerMarketData);
	lpExecutor->AddExecution(lExecutorInput);
}

AT::ExecutorInput AccountSignalDispach::BuildExecutorInput(const SignalExecutInfo& lInfo, const MarketData& aTriggerMarket )
{
	ExecutorInput lret;
	strcpy_s(lret.InstrumentID,cInstrimentIDLength,lInfo.InstrumentID.c_str());
	lret.BuySellCode = lInfo.BuySellCode;
	lret.OpenCloseCode = lInfo.OpenCloseCode;
	lret.vol = m_Vol;
	lret.TriggerMarketData = aTriggerMarket;
	return lret;

}

void AccountSignalDispach::LoadExecuteInfoFromFile( const std::string& aConfigfile )
{
	boost::property_tree::ptree lConfig;
	read_xml(aConfigfile,lConfig);
	for( std::pair<std::string,boost::property_tree::ptree>  lSignalHandleItem : lConfig.get_child("SignalDispatch"))
	{

		std::string lSignalID = lSignalHandleItem.second.get<std::string>("SignalID");
		std::string lBuySellString = lSignalHandleItem.second.get<std::string>("BuySellCode");
		std::string lOpenCloseString = lSignalHandleItem.second.get<std::string>("OpenCloseCode");
		std::string lInstrumentID =  lSignalHandleItem.second.get<std::string>("InstrumentID");
		std::string lExecutorID = lSignalHandleItem.second.get<std::string>("ExecutorID");

		SignalExecutInfo lOneSignalCase;
		lOneSignalCase.BuySellCode = lBuySellString=="Buy" ? BuySellType::BuyOrder : BuySellType::SellOrder;
		lOneSignalCase.OpenCloseCode = lOpenCloseString == "Open" ? OpenCloseType::Open : OpenCloseType::Close;
		lOneSignalCase.ExecutorID = lExecutorID;
		lOneSignalCase.InstrumentID = lInstrumentID;
		lOneSignalCase.vol = m_Vol;

		m_Signal_Executor_Map[lSignalID] = lOneSignalCase;
	}
}


}