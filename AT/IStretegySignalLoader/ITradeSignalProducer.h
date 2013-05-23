#pragma once
#include "MarketData.h"
namespace AT
{


class ITradeSignalProducer
{
public:
	//this module is work as Strategy loader before , select as �ź�ID
	//��һ����ⲿ����ָ��ʵ��
	virtual bool OnMarketDepth(const MarketData& aMarketDepth) = 0;

};

}