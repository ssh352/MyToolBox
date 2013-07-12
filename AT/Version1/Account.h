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

	void InitExecutorContainer(const std::string& aExecutoConfigFile);

	void InitSignalDispatcher(const std::string& aDispatcherConfigFile);
private:
	void HandleCommand(Command aCommand);
	void HandleExecutorResult(ExecutionResult);

	IDriver_TD* m_pTD;
	int			m_AccountVol;		//ÿ���źŷ��͵ı���
	std::string m_AccountID;

	std::unique_ptr<ISignalFliter>								m_pFilter;
	std::unique_ptr<ExecutorContianer>						m_pExecutorContianer;
	std::unique_ptr<AccountSignalDispach>					m_pAccountSignalDispach;


};

}