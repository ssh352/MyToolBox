#pragma once
#include "ITradeSpi.h"
class PrintTDSpi : public AT::ITradeSpi
{
public:

	virtual void NotifyStateTD(AT::ETradeState aErrorCode,const char* aErrorMsg) override;
	virtual void OnRtnOrder(const AT::OrderUpdate& apOrder) override;
	virtual void OnRtnTrade(const AT::TradeUpdate& apTrade) override;

};

