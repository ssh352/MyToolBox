#include "TradeAccountDemo1.h"
namespace AT
{

	TradeAccountDemo1::TradeAccountDemo1( const std::string& aConfigFile, IDriver_TD* apTD,ITradeAccountObserver* apTradeAccountOB )
		:m_pTD(apTD)
		,m_pTradeAccountOB(apTradeAccountOB)
	{
		
	}

	TradeAccountDemo1::~TradeAccountDemo1(void)
	{
	}

	void TradeAccountDemo1::HandleTradeSignal( const TradeSignal& aTradeSignal )
	{

	}

	void TradeAccountDemo1::OnMarketDepth( const MarketData& aMarketDepth )
	{

	}

	void TradeAccountDemo1::OnRtnOrder( const OrderUpdate& apOrder )
	{

	}

	void TradeAccountDemo1::OnRtnTrade( const TradeUpdate& apTrade )
	{

	}

	void TradeAccountDemo1::InitFromConfigFile( const std::string& aConfigFile )
	{
		//todo load AccountID
		m_AccountID;

		//todo Load Signal OpenExecutor Map

		//todo Load Close Executor
	}

}