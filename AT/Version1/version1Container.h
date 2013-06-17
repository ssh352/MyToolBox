#pragma once

#include "AT_Struct.h"
#include <vector>
namespace AT
{
class IndexContainer;
class ITradeSignalProducer;
class ITradeSignalFliter;

class version1Container
{
public:
	version1Container(void);
	~version1Container(void);

	//feed data
	void OnMarketDepth(const AT::MarketData& aMarketDepth) ;

private:
	IndexContainer* m_pIndexContaner;
	std::vector<ITradeSignalProducer* > m_TradeSignalProducerVec;
	ITradeSignalFliter*					m_pTradeSignalFliter;

};

}