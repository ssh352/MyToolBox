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
	//����������
	int						    m_LimitMaxVol;//�޼۵��������
	int					        m_MarketMaxVol;//�м۵��������
	int							m_MaxCancleTime;//����������
	int							m_TotalMaxOpenVol;//����ۼƿ�������
	int							m_SinglePositionVol;//���ֲ߳��޶�
	int							m_AutoTradeMaxTime;//�Գɽ�������

	int						    m_SellDirectionVol;//���ֲ�����
	int						    m_BuyDirectionVol;//��ֲ�����
	int						    m_TotalOpenVol;//����ۼƿ�������
	int						    m_TotalCancleTime;//����������
	int						    m_AutoTradeTime;//�Գɽ�������
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

	//�����������趨
private:
	//��ȡ����������
	void InitExchangeRule(const std::string& aConfigFile);

	//���潻����
	void StoreTradeVol();
	//�ظ����潻����
	void RestoreTradeVol();

	//���õ��ֲ߳��޶�
	void SetSignalDirectionVol(BuySellType type,int Vol,bool bAdd);
	
	//��������ۼƿ�������
	void SetTotalOpenVol(int vol){m_ExechangeSetting.m_TotalOpenVol += vol;StoreTradeVol();}

	//���ó������״���
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