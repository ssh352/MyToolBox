#pragma once
#include "AT_Struct.h"
namespace CTP
{
class EnumTranslator
{
public:
	static void Init();

	static	char TransOrderPriceType(AT::OrderPriceType);
	static	AT::OrderPriceType	TransOrderPriceType(char);

	static	char	TransBuySellType(AT::BuySellType);
	static	AT::BuySellType		TransBuySellType(char);

	static	char	TransTriggerType(AT::TriggerType);
	static	AT::TriggerType TransTriggerType(char);

	static	char	TransOpenCloseType(AT::OpenCloseType);
	static	AT::OpenCloseType	TransOpenCloseType(char);

	static	char	TransTimeType(AT::TimeInForceType);
	static	AT::TimeInForceType TransTimeType(char);

	static char TransOrderStatusType(AT::OrderStatusType);
	static AT::OrderStatusType	TransOrderStatusType(char);

	static char TransOrderSubmitStatusType(AT::OrderSubmitStatusType);
	static AT::OrderSubmitStatusType TransOrderSubmitStatusType(char);
};

}