#pragma once
#include "ExecutorBase.h"
#include "IDriver_TD.h"
#include <set>

#include <boost\property_tree\ptree.hpp>
#include <boost\property_tree\xml_parser.hpp>
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

class CloseExecutor_3Level :public ExecutorBase
{
public:
	CloseExecutor_3Level(const boost::property_tree::ptree& aConfigPtee);
	virtual ~CloseExecutor_3Level(void);


	//����1 �������ϲ�Ľ����ź�
	virtual void DoAddExecution(ExecutorInput aExecutorInput) override;
	virtual void  DoAbrot() override;
	virtual	void  DoOnMarketDepth(const AT::MarketData& aMarketDepth) override;
	virtual	void  DoOnRtnOrder(const  AT::OrderUpdate& apOrder)override;
	virtual	void  DoOnRtnTrade(const  AT::TradeUpdate& apTrade)override;

private:


	//todo Init Executor;

	bool	CheckIsFinished();
	boost::shared_ptr<IExecutor>		m_pFirstExecutor;		//�յ��������ƽ����������֮ǰ��Executor
	boost::shared_ptr<IExecutor>		m_pQuitExecutor;		//����ƽ�ֺ󣬾���Ҫ����ʹ�õ�ƽ��Executor
	bool	CheckTrigger(const AT::MarketData& aMarketDepth); //�ж��Ƿ���Ҫ����ƽ���߼�
	ExecutorInput BuildQuitExecution();						//���ݱ���������Ҫ������QuitVolָ��

	void UpdatePriceLevel( int AbsPriceDiff );

	void	HandleFirstExecutorResult(ExecutionResult aTrade);
	void	HandleQuitExecutorResult(ExecutionResult aTrade);
	void InitCheckLevelSetting( const boost::property_tree::ptree &lConfigPtree );
	void InitChildExecutor( const boost::property_tree::ptree &lConfigPtree );


	bool				m_IsTriggered;
	int					m_TradeQuantity;
	ExecutorInput		m_ExecutorInput;

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

	enum class Close3LevelStatus
	{
		BeforeStart,
		StopLoss_CheckTrigger,
		PendingStopLossCancel_Time,
		QuitExecutor_Time,
		AbrottingTime,
		Finished,
	};

	Close3LevelStatus		m_StatusEnumCode;
};

}
