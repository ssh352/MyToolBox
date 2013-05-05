#pragma once

#include <boost\cstdint.hpp>
#include <boost\date_time.hpp>
#include <boost\date_time\posix_time\ptime.hpp>
#include <boost\uuid\uuid.hpp>
#include  <limits.h>

namespace AT
{
	using boost::uint32_t;
	using boost::int32_t;

	static const  int32_t  AT_INVALID_PRICE = _I32_MAX;
	static const uint32_t AT_INVALID_VOL = _UI32_MAX;
	
	typedef boost::posix_time::ptime AT_Time;
	std::string ToString(const AT_Time& aTime);

	extern const AT_Time AT_INVALID_TIME;
	AT_Time AT_Local_Time();
	AT_Time AT_UTC_Time();

	static const int cInstrimentIDLength = 32;
	static const int cAccoutIDLength = 16;
	static const int cTradeIDLength = 21;
	static const int cPlatformOrderIDLength = 81;
	static const int cExchangeOrderIDLength = 81;
	static const int cErrorMessageLength = 81;	//same with CTP same fields


	enum class BuySellType
	{
		BuyOrder,
		SellOrder,
	};
	std::string ToString(BuySellType aType);

	enum class OrderType
	{
		LimitOrder,
		MarketOrder
	};
	std::string ToString(OrderType aType);


	//todo when need add
	enum class TimeInForceType
	{
		IOC,
		//....
	};
	std::string ToString(TimeInForceType aType);

	enum class OpenCloseType
	{
		OpenOrder,
		CloseOrder,
		CloseTodayOrder
	};
	std::string ToString(OpenCloseType aType);

	enum class OrderStatusType
	{
		UnHandleOrder,
		RejectOrder,
		ActiveOrder,
		StoppedOrder,
	};
	std::string ToString(OrderStatusType aType);

	typedef 	boost::uuids::uuid AT_Order_Key;
	std::string ToString(AT_Order_Key aKey);

	double TransPriceToDouble(int32_t aATPrice);
	int32_t TranPriceToInt(double aExchangePrice);



}

