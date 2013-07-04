#pragma once

#include "ITradeSignalProducer.h"

#include <boost\function.hpp>
#include <string>
#include <vector>
namespace AT
{
class IndexContainer;

//load the logic from script as python
class TradeSignalProducerDemo1 :public ITradeSignalProducer
{
public:
	TradeSignalProducerDemo1(const std::string& aConfigFile,IndexContainer* apIndexContainer);
	virtual ~TradeSignalProducerDemo1(void);
	virtual TradeSignal ProduceTradeSignal(const MarketData& aTriggerMakret) override;

	//初始化
	void	InitConfig(const std::string& aConfigFile);
	//记录信号
	void    WriteTradeSignal();

private:
	IndexContainer* m_pIndexContainer;

	int m_Seqence;

	std::string		m_SignalName;
	bool			m_IsBuy;
	int32_t			m_Priority;


	static const int CountCheckType  = 1;
	static const int LastValCheckType = 2;
	static const int LastNonZeroCheckType  = 3;

	typedef boost::function<bool( AT_Time aTime)> CheckFunction;
	std::vector<CheckFunction>		m_CheckList;
	std::vector<TradeSignal>		m_TradeSignalVec;
};

}
