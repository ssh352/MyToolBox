#pragma once
#include "IAccount.h"
#include <string>
#include <boost\smart_ptr.hpp>
#include "SingleDBWriter.h"
#include "Command.h"
#include "IExecutor.h"
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

	void IntiSignalHandleSet();

	void DoTradeCommand(Command apTradeCommand);


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
	int			m_AccountVol;		//ÿ���źŷ��͵ı���
	std::string m_AccountID;
private:


	void HandleCommand(Command aCommand);
	void HandleExecutorResult(ExecutionResult);

	IDriver_TD* m_pTD;

	std::unique_ptr<ExecutorContianer>						m_pExecutorContianer;

	ExechangeRule				m_ExechangeSetting;
	std::string					m_ExchangePath;
	std::shared_ptr<SingleDBHandler>  m_TradeVolDB;

};

}