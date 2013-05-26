#pragma once
#include "MarketData.h"
#include "TradeSignal.h"
namespace AT
{


class ITradeSignalProducer
{
public:
	//���캯���õ���ʷ����
	//this module is work as Strategy loader before , select as �ź�ID
	//��һ����ⲿ����ָ��ʵ��
	virtual std::pair<bool,TradeSignal>  OnMarketDepth(const MarketData& aMarketDepth) = 0;
	 
};

}