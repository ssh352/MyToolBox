#pragma once
#include "ITradeAccount.h"
#include <string>
#include <boost\smart_ptr.hpp>
namespace AT
{
class IDriver_TD;
class ITradeAccountObserver;
class ITradeSignalExecutor;
struct TradeCommand;

class TradeAccountDemo1 :public ITradeAccount
{
public:
	TradeAccountDemo1(const std::string& aConfigFile, IDriver_TD* apTD);
	virtual ~TradeAccountDemo1(void);
	virtual void HandleTradeSignal(const TradeSignal& aTradeSignal) override;
	virtual void OnMarketDepth(const MarketData& aMarketDepth) override;
	virtual void OnRtnOrder(const  OrderUpdate& apOrder) override;
	virtual void OnRtnTrade(const  TradeUpdate& apTrade) override;  
	virtual void SetProfitCallback(TradeSignalProfitTimeNotifyer aTradeSignalProfitTimeNotifyerCallback) override
	{
		m_ProfitNotifyer = aTradeSignalProfitTimeNotifyerCallback;
	}; 
private:
	void InitFromConfigFile(const std::string& aConfigFile);
	void DoTradeCommand(boost::shared_ptr<TradeCommand> apTradeCommand);

	void HandleCloseExecutorResult(int32_t aPrice, int32_t aVol,bool IsBuy,bool isFinishe);
	void HandleOpenExecutorResult(int32_t aPrice, int32_t aVol,bool IsBuy, bool isFinishe);


private:
	int			m_TargetVol;
	std::string m_AccountID;
private:
	IDriver_TD* m_pTD;
	std::map<std::string,boost::shared_ptr<ITradeSignalExecutor> > m_OpenExecutorMap;
	//boost::shared_ptr<ITradeSignalExecutor>							m_OpenExecutor;;
	boost::shared_ptr<ITradeSignalExecutor>							m_CloseExecutor;
	std::string			m_openExecutorID;

	MarketData			m_LastMarket;
	TradeSignal			m_LastTradeSignal ;
	TradeSignalProfitTimeNotifyer m_ProfitNotifyer;
	bool m_IsCompleteOpen;
	bool m_IsCompleteClose;

	int m_totalProfit;

	

};

}