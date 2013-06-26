#include "TradeSignalProducerDemo1.h"
#include "IndexContainer.h"
namespace AT
{


TradeSignalProducerDemo1::~TradeSignalProducerDemo1(void)
{
}

TradeSignalProducerDemo1::TradeSignalProducerDemo1(const std::string& aConfigFile, IndexContainer* apIndexContainer )
	:m_pIndexContainer(apIndexContainer)
	,m_Seqence(0)
{

}

AT::TradeSignal TradeSignalProducerDemo1::ProduceTradeSignal( const MarketData& aTriggerMarket )
{
		TradeSignal lret ;
		int iIndexCountB = m_pIndexContainer->GetIndexCount("HKY006",1,aTriggerMarket.m_UpdateTime,aTriggerMarket.m_UpdateTime);
		int iIndexCountS = m_pIndexContainer->GetIndexCount("HKY006",-1,aTriggerMarket.m_UpdateTime,aTriggerMarket.m_UpdateTime);
		if( iIndexCountB > iIndexCountS && iIndexCountB >=1 )
		{
			lret.m_Valid = true;
			strcpy_s(lret.m_ID,g_TradeSignalIDLength,"HKY006");
			lret.m_BuyOrSell = true;
			lret.m_priority = 100;
			lret.m_TriggerMarketData = aTriggerMarket;
			lret.m_TradeSignalSequence = m_Seqence;
			lret.m_TradeSignalType = m_Seqence++;
			return lret;
		}
		else
		{
			lret.m_Valid = true;
			strcpy_s(lret.m_ID,g_TradeSignalIDLength,"HKY006");
			lret.m_BuyOrSell = false;
			lret.m_priority = 100;
			lret.m_TriggerMarketData = aTriggerMarket;
			lret.m_TradeSignalSequence = m_Seqence;
			lret.m_TradeSignalType = m_Seqence++;
			return lret;
		}

		lret.m_Valid = false;
		return lret;
}

}
