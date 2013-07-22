#pragma once
#include "IAccount.h"
#include "SingleDBWriter.h"
#include "Command.h"
#include "IExecutor.h"

#include "FilterSystemLevel.h"


#include <boost\asio.hpp>
#include <string>
#include <boost\smart_ptr.hpp>
#include <set>

namespace AT
{
class IDriver_TD;
class ITradeAccountObserver;
class IExecutor;
class ExecutorContianer;
class AccountSignalDispach;
class ISignalFliter;



class Account :public IAccount
{
public:
	Account(const std::string& aConfigFile, IDriver_TD* apTD);
	virtual ~Account(void);
	virtual void HandleTradeSignal(const Signal& aTradeSignal) override;



	void DoNewSignal( const Signal &aSignal );

	virtual void OnMarketDepth(const MarketData& aMarketDepth) override;
	virtual void OnRtnOrder(const  OrderUpdate& apOrder) override;



	virtual void OnRtnTrade(const  TradeUpdate& apTrade) override;  
private:

	void CheckCancelTimes( const OrderUpdate &apOrder );
	void InitExechangeRule(const std::string& aConfig);
	void InitFromConfigFile(const std::string& aConfigFile);

	void InitExecutorContainer(const std::string& aExecutoConfigFile);

	void InitSignalDispatcher(const std::string& aDispatcherConfigFile);
private:
	void HandleCommand(Command aCommand);
	void HandleExecutorResult(ExecutionResult);
	bool TryToFilter();

	IDriver_TD* m_pTD;
	int			m_AccountVol;		//每个信号发送的倍数
	std::string m_AccountID;



	struct ExecutorPara
	{
		std::string Type;
		std::string ConfigFile;
	};

	//每个信号对应的执行信息
	struct SignalExecutInfo
	{
		std::string		 ExecutorID;
		std::string  InstrumentID;
		BuySellType BuySellCode;
		OpenCloseType OpenCloseCode;
		int			vol;
	};
	
	enum class AccountStatus
	{
		IDLE,
		OnSignal,
		OnCloseStatus_Fellow,
		OnCloseStatus_Market,
		Done,
	};

	//每一套预案对应的ExecutorID与其参数
	std::map<std::string,ExecutorPara>			m_HandleInfoGroup;
	std::map<std::string,SignalExecutInfo>		m_Signal_Executor_Map;
	boost::shared_ptr<IExecutor>					m_CurrentExecutor;

	AccountStatus								m_Status ;


	struct ExechangeRule
	{
		//交易所规则
		int						    LimitMaxVol;//限价单最多手数
		int					        MarketMaxVol;//市价单最多手数
		int							MaxCancleTimeVol;//撤单最多次数
		int							MaxTotalOpenVol;//最大累计开仓手数
		int							SinglePositionMaxVol;//单边持仓限额
		int							AutoTradeMaxTime;
	};
	ExechangeRule				m_ExchangeRule;

	std::set<AT_Order_Key>		m_OrderKeyList;
	int							m_CancelTimes;



	int							m_PorfitOfCurrentSignal;
	std::map<AT_Time,int32_t> m_ProfitStatusMap;


	struct LastNTradeStatus
	{
		bool m_isAllLoss;
		int	m_TotalLoss;
		AT_Time m_TimeLoss;
	};
	LastNTradeStatus IsLastNTradeLoss(int i);
	AT_Time m_LastTime;
	FliterStruct		m_FilterSetting;

};

}