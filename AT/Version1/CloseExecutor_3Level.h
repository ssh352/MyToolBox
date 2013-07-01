#pragma once
#include "ITradeSignalExecutor.h"
#include "IDriver_TD.h"
#include <set>
namespace AT
{

struct CloseSetting_3Level
{
	int StopLossOrderPrice; //���ȵ�ֹ��۸��
	int QuitLevel_0;
	int EnterLevel_1;
	int QuitLevel_1;
	int EnterLevel_2;
	int QuitLevel_2;
	int EnterLevel_3;
	int QuitLevel_3;
	AT_Time StopTime;//�涨ʱ��㻹�гֲ֣���ȫ��ƽ��
	AT_Time StopClearTime;//�涨ʱ��㻹�гֲ֣����м����
};

class CloseExecutor_3Level :public ITradeSignalExecutor
{
public:
	CloseExecutor_3Level(CloseSetting_3Level);
	virtual ~CloseExecutor_3Level(void);

	//����1 �������ϲ�Ľ����ź�
	virtual boost::shared_ptr<TradeCommand> SetupTarget(int targetQuantity,bool isBuy, const AT::MarketData& aMarket) override;
	virtual boost::shared_ptr<TradeCommand> AddTarget(int addTargetQuantity, bool isBuy,const AT::MarketData& aMarket) override;

	//����2 ������ִ�в���
	virtual  boost::shared_ptr<TradeCommand> OnMarketDepth(const AT::MarketData& aMarketDepth) override;
	virtual  boost::shared_ptr<TradeCommand> OnRtnOrder(const  AT::OrderUpdate& apOrder)override;
	virtual  boost::shared_ptr<TradeCommand> OnRtnTrade(const  AT::TradeUpdate& apTrade)override;
	virtual std::string GetExecutorID() override;



private:
	//ֹӯֹ��(�����������ȵ�)
	boost::shared_ptr<TradeCommand> PlaceStopLossOrder(const AT::MarketData& aMarket);
	//ƽ�ֵ�
	boost::shared_ptr<TradeCommand> PlaceCloseOrder(const AT::MarketData& aMarket);

	boost::shared_ptr<TradeCommand> DoQuit(const AT::MarketData& aMarket);

	int			m_CurrentLevel;
	int			m_StartPrice;
	int			m_PriceCheck;
	int			m_TargetVol;
	int			m_ActiveOrderVol;
	CloseSetting_3Level	m_Setting;
	AT_Order_Key				m_DelOrderSet;
	AT::MarketData m_LastMarket;
	int				m_LastLossPrice;
	AT_Time			m_TriggerLossTime;

private:
	bool		m_IsBuy;
	std::set<AT_Order_Key>		m_SendOrderSet;
	std::set<AT_Order_Key>		m_SendLossSet;

	enum class Status
	{
		IDLE,
		WaitForStopLossOrderPlace,
		WaitQuitSignal,
		WaitForStopLossOrderCancel,
		WaitForNewQuitOrder,
	};
	Status					m_Status;
};

}