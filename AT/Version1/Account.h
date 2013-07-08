#pragma once
#include "IAccount.h"
#include <string>
#include <boost\smart_ptr.hpp>
#include "SingleDBWriter.h"
#include "Command.h"
namespace AT
{
class IDriver_TD;
class ITradeAccountObserver;
class IExecutor;
class ExecutorContianer;

struct ExechangeRule
{
	//交易所规则
	int						    m_LimitMaxVol;//限价单最多手数
	int					        m_MarketMaxVol;//市价单最多手数
	int							m_MaxCancleTime;//撤单最多次数
	int							m_TotalMaxOpenVol;//最大累计开仓手数
	int							m_SinglePositionVol;//单边持仓限额
	int							m_AutoTradeMaxTime;//自成交最大次数

	int						    m_SellDirectionVol;//卖持仓手数
	int						    m_BuyDirectionVol;//买持仓手数
	int						    m_TotalOpenVol;//最大累计开仓手数
	int						    m_TotalCancleTime;//撤单最多次数
	int						    m_AutoTradeTime;//自成交最大次数
};

class Account :public IAccount
{
public:
	Account(const std::string& aConfigFile, IDriver_TD* apTD);
	virtual ~Account(void);
	virtual void HandleTradeSignal(const Signal& aTradeSignal) override;
	virtual void OnMarketDepth(const MarketData& aMarketDepth) override;
	virtual void OnRtnOrder(const  OrderUpdate& apOrder) override;
	virtual void OnRtnTrade(const  TradeUpdate& apTrade) override;  
private:
	void InitFromConfigFile(const std::string& aConfigFile);

	void InitExchangeStroe();

	void InitExecutorContainer(const std::string& aExecutoConfigFile);

	void DoTradeCommand(Command apTradeCommand);

	void HandleCloseExecutorResult(int32_t aPrice, int32_t aVol,bool IsBuy,bool isFinishe);
	void HandleOpenExecutorResult(int32_t aPrice, int32_t aVol,bool IsBuy, bool isFinishe);

	//交易所规则设定
private:
	//获取交易所规则
	void InitExchangeRule(const std::string& aConfigFile);

	//保存交易数
	void StoreTradeVol();
	//回复保存交易数
	void RestoreTradeVol();

	//设置单边持仓限额
	void SetSignalDirectionVol(BuySellType type,int Vol,bool bAdd);
	
	//设置最大累计开仓手数
	void SetTotalOpenVol(int vol){m_ExechangeSetting.m_TotalOpenVol += vol;StoreTradeVol();}

	//设置撤单交易次数
	void SetTotalCancleTime(){m_ExechangeSetting.m_TotalCancleTime++;StoreTradeVol();}

private:
	int			m_TargetVol;
	std::string m_AccountID;
private:
	IDriver_TD* m_pTD;

	std::unique_ptr<ExecutorContianer>						m_pExecutorContianer;

	//std::map<std::string,boost::shared_ptr<IExecutor> > m_OpenExecutorMap;
	////boost::shared_ptr<ITradeSignalExecutor>							m_OpenExecutor;;
	//boost::shared_ptr<IExecutor>							m_CloseExecutor;
	//std::string			m_openExecutorID;

	//MarketData			m_LastMarket;
	//Signal			m_LastTradeSignal ;
	//bool m_IsCompleteOpen;
	//bool m_IsCompleteClose;

	//int m_totalProfit;

	ExechangeRule				m_ExechangeSetting;
	std::string					m_ExchangePath;
	std::shared_ptr<SingleDBHandler>  m_TradeVolDB;

};

}