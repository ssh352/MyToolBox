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
	extern const AT_Time AT_INVALID_TIME;
	AT_Time AT_Local_Time();
	AT_Time AT_UTC_Time();

	static const int cInstrimentIDLength = 8;
	static const int cAccoutIDLength = 16;
	static const int cTradeIDLength = 16;


	enum class BuySellType
	{
		BuyOrder,
		SellOrder,
	};

	enum class OrderType
	{
		LimitOrder,
		MarketOrder
	};


	//todo when need add
	enum class TimeInForceType
	{
		IOC,
		//....
	};

	enum class OpenCloseType
	{
		OpenOrder,
		CloseOrder,
		CloseTodayOrder
	};

	enum class OrderStatusType
	{
		//reject order not consider

		ActiveOrder,
		StoppedOrder,
	};

	typedef 	boost::uuids::uuid AT_Order_Key;
	double TransPriceToDouble(int32_t aATPrice);
	int32_t TranPriceToInt(double aExchangePrice);



}

