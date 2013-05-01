#pragma once
#include "IMarketSpi.h"
class PrintMDSpi :public AT::IMarketSpi
{
public:
	virtual void NotifyStateMD(AT::EMarketState aErrorCode,const char* aErrorMsg) override;

	virtual void NotifyMarketDepth(const AT::MarketData& aMarketDepth) override;
};

