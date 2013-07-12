#pragma once
#include "IAccount.h"
#include "SingleDBWriter.h"
#include "Command.h"
#include "IExecutor.h"

#include <boost\asio.hpp>
#include <string>
#include <boost\smart_ptr.hpp>

namespace AT
{
class IDriver_TD;
class ITradeAccountObserver;
class IExecutor;
class ExecutorContianer;
class AccountSignalDispach;
class ISignalFliter;

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

	void InitExecutorContainer(const std::string& aExecutoConfigFile);

	void InitSignalDispatcher(const std::string& aDispatcherConfigFile);
private:
	void HandleCommand(Command aCommand);
	void HandleExecutorResult(ExecutionResult);

	IDriver_TD* m_pTD;
	int			m_AccountVol;		//每个信号发送的倍数
	std::string m_AccountID;

	std::unique_ptr<ISignalFliter>								m_pFilter;
	std::unique_ptr<ExecutorContianer>						m_pExecutorContianer;
	std::unique_ptr<AccountSignalDispach>					m_pAccountSignalDispach;


};

}