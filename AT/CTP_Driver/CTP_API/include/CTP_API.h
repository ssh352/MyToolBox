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


namespace CTP
{
	typedef boost::shared_ptr<CThostFtdcInputOrderField> InputOrderTypePtr;
	typedef boost::shared_ptr<CThostFtdcOrderField> OrderTypePtr;
	typedef boost::shared_ptr<CThostFtdcTradeField> TradeTypePtr;

	std::string GenerateThostOrderID(InputOrderTypePtr aOrderPtr,int aFrontID,int aSessionID);
	std::string GenerateThostOrderID(OrderTypePtr aOrderPtr);
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
}


