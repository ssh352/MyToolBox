#pragma once
#include "IExecutor.h"
#include "IDriver_TD.h"
#include <set>

namespace AT
{

struct CloseSetting_3Level
{
	int EnterLevel_1;
	int QuitLevel_1;
	int EnterLevel_2;
	int QuitLevel_2;
	int EnterLevel_3;
	int QuitLevel_3;
};

class CloseExecutor_3Level :public IExecutor
{
public:
	CloseExecutor_3Level(const std::string& aConfigFile);
	virtual ~CloseExecutor_3Level(void);


	//输入1 来自于上层的交易信号
	virtual void AddExecution(ExecutorInput aExecutorInput) override;
	virtual void Abrot() override;

	//输入2 来自于执行层面
	virtual	void OnMarketDepth(const AT::MarketData& aMarketDepth) override;
	virtual	void OnRtnOrder(const  AT::OrderUpdate& apOrder)override;
	virtual	void OnRtnTrade(const  AT::TradeUpdate& apTrade)override;

	
	virtual ExecutionStatus	GetExecutionStatus() override;
	virtual std::string GetExecutorID()  override;


private:


	//todo Init Executor;

	bool	CheckIsFinished();
	boost::shared_ptr<IExecutor>		m_pFirstExecutor;		//收到请求后，在平常条件触发之前的Executor
	boost::shared_ptr<IExecutor>		m_pQuitExecutor;		//决定平仓后，决定要具体使用的平仓Executor
	bool	CheckTrigger(const AT::MarketData& aMarketDepth); //判断是否需要进入平仓逻辑

	void UpdatePriceLevel( int AbsPriceDiff );

	void	HandleFirstExecutorResult(ExecutionResult aTrade);
	void	HandleQuitExecutorResult(ExecutionResult aTrade);
	void	InitFromConfigFile(const std::string& aConfig);


	ExecutionStatus		m_Status;
	bool				m_IsTriggered;

	//Check Trigger member
	int					m_StartPrice;
	int					m_MaxPriceDiff;
	
	enum class CheckStatusLevel
	{
		Level0,
		Level1,
		Level2,
		Level3,
	};
	CheckStatusLevel m_CurrentLevel;
	BuySellType			m_BuySellCode;
	CloseSetting_3Level	m_Setting;
	std::string			m_InstrumentID;


};

}
