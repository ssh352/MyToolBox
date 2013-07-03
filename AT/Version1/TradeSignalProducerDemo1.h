#pragma once

#include "ITradeSignalProducer.h"
#include <string>
namespace AT
{
class IndexContainer;

//todo config files
struct strSignal
{
	std::string  SignalName;
	bool		 BuyOrSell;
	int			 Priority;
	int			 HKY006IsUse;
	std::string	 HKY006ItemName;
	int			 HKY006ReturnValue;
};

//load the logic from script as python
class TradeSignalProducerDemo1 :public ITradeSignalProducer
{
public:
	TradeSignalProducerDemo1(const std::string& aConfigFile,IndexContainer* apIndexContainer);
	virtual ~TradeSignalProducerDemo1(void);
	virtual TradeSignal ProduceTradeSignal(const MarketData& aTriggerMakret) override;

	//��ʼ��
	void	InitConfig(const std::string& aConfigFile);
	//��¼�ź�
	void    WriteTradeSignal(TradeSignal signal);

	bool    CheckSignal(AT_Time aTriggerTime);

private:
	IndexContainer* m_pIndexContainer;

	int m_Seqence;

	std::vector<strSignal>   m_Signal;

	
};

}
