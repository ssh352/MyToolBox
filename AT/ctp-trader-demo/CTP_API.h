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
	typedef boost::shared_ptr<CThostFtdcOrderField> OrderTypePtr;
	typedef boost::shared_ptr<CThostFtdcTradeField> TradeTypePtr;
	std::string GenerateExchangeOrderID(OrderTypePtr aOrderPtr);
	std::string GenerateClientOrderID(OrderTypePtr aOrderPtr);
	std::string GenerateTradeID(TradeTypePtr aTradePtr);
}


