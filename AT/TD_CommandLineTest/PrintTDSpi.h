#pragma once
#include "ITradeSpi.h"
class PrintTDSpi : public AT::ITradeSpi
{
public:
	virtual void OnRtnState(int aErrorCode,const std::string& aErrorMsg);
	virtual void OnRtnOrder(const std::string& apOrder);
	virtual void OnRtnTrade(const std::string& apTrade);
	virtual void OnRtnPosition(const std::string& aPosition);

};

