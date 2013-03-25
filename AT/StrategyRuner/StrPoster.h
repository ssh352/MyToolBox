#pragma once
#include "IMarketSpi.h"
#include "StrDemo1.h"
class StrPoster :public AT::IMarketSpi
{
public:
	virtual void NotifyState(int aErrorCode,const std::string& aErrorMsg) ;
	virtual void NotifyExchange(const std::string& aExchange) ;
	virtual void NotifyProduct(const std::string& aProduct) ;
	virtual void NotifyInstrument(const std::string& aInstrument);
	virtual void NotifyMarketDepth(const std::string& aMarketDepth);

private:

	StrDemo1 m_Inst;
};

