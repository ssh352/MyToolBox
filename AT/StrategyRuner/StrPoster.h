#pragma once

#include "IMarketSpi.h"
#include  "ITradeSpi.h"
#include "StrDemo1.h"

namespace AT
{
	class IDriver_TD;

}
class StrPoster :public AT::IMarketSpi , public AT::ITradeSpi
{
public:
	StrPoster();
	virtual void NotifyState(int aErrorCode,const std::string& aErrorMsg) ;
	virtual void NotifyExchange(const std::string& aExchange) ;
	virtual void NotifyProduct(const std::string& aProduct) ;
	virtual void NotifyInstrument(const std::string& aInstrument);
	virtual void NotifyMarketDepth(const std::string& aMarketDepth);

public:
	virtual void OnRtnState(int aErrorCode,const std::string& aErrorMsg);
	virtual void OnRtnOrder(const std::string& apOrder);
	virtual void OnRtnTrade(const std::string& apTrade);
	virtual void OnRtnPosition(const std::string& aPosition);
//
//public:
//	void SetTDHandle(AT::IDriver_TD* apTD)
//	{
//		m_pTD = apTD;
//	}
//
//private:
//	AT::IDriver_TD* m_pTD;
	StrDemo1 m_Inst;
};

