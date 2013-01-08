#pragma  once

#pragma pack(push)
#pragma pack(8)
#include "ThostFtdcMdApi.h"
#include "ThostFtdcTraderApi.h"
#include "ThostFtdcUserApiDataType.h"
#include "ThostFtdcUserApiStruct.h"
#pragma pack(pop)

#include <string>
#include <boost\shared_ptr.hpp>
#include <boost\lexical_cast.hpp>

namespace CTP
{
	typedef boost::shared_ptr<CThostFtdcInputOrderField> InputOrderTypePtr;
	typedef boost::shared_ptr<CThostFtdcOrderField> OrderTypePtr;
	typedef boost::shared_ptr<CThostFtdcTradeField> TradeTypePtr;

	std::string GenerateThostOrderID(InputOrderTypePtr aOrderPtr,int aFrontID,int aSessionID);

	template<typename TorderPtr>
	std::string GenerateThostOrderID( TorderPtr aOrderPtr )
	{
		std::string lRet;
		lRet += boost::lexical_cast<std::string>(aOrderPtr->SessionID);
		lRet += '_';
		lRet += boost::lexical_cast<std::string>(aOrderPtr->FrontID);
		lRet += '_';
		lRet += boost::lexical_cast<std::string>(aOrderPtr->OrderRef);
		return lRet;
	}

	template<typename Order_TradeTypePtr>
	std::string GenerateExchangeOrderID(Order_TradeTypePtr aOrderPtr)
	{
		std::string lRet;
		lRet += std::string(aOrderPtr->ExchangeID,8);
		lRet += std::string( aOrderPtr->OrderSysID,20);
		return lRet;
	}
	std::string GenerateClientOrderID(OrderTypePtr aOrderPtr);
	std::string GenerateTradeID(TradeTypePtr aTradePtr);
	bool IsErrorRspInfo(CThostFtdcRspInfoField *pRspInfo);
	std::string ResolveThostOrderID(const std::string& lThostOrderID,int& ar_SessionID, int& ar_FrointID);
}


