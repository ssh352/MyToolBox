#include "TradeSignalProducerDemo1.h"
#include "IndexContainer.h"
#include "boost/format.hpp"
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
		int iIndex = m_pIndexContainer->GetIndex("HKY006",7,aTriggerMarket.m_UpdateTime-boost::posix_time::seconds(5),aTriggerMarket.m_UpdateTime);
		//std::cout<<boost::format("lresult:%d")%iIndex<<std::endl;
		if( iIndex == 1 || iIndex == -1 )
		{
			lret.m_Valid = true;
			strcpy_s(lret.m_ID,g_TradeSignalIDLength,"HKY006");
			lret.m_BuyOrSell = iIndex == 1 ?true : false;
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
