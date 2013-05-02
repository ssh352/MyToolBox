#include "ConstVarDefine.h"
#include <boost\uuid\uuid_io.hpp>
namespace AT
{

	extern const AT_Time AT_INVALID_TIME = boost::posix_time::ptime(boost::posix_time::not_a_date_time);


	AT_Time AT_Local_Time()
	{
		return boost::posix_time::microsec_clock::universal_time();
	}

	AT_Time AT_UTC_Time()
	{
		return boost::posix_time::microsec_clock::local_time();
	}

	double TransPriceToDouble( int32_t aATPrice )
	{
		return aATPrice * 0.001;
	}

	int32_t TranPriceToInt( double aExchangePrice )
	{
		return int32_t(aExchangePrice*1000 + 0.1);
	}

	std::string ToString( BuySellType aType )
	{
		switch (aType)
		{
		case AT::BuySellType::BuyOrder:
			return std::string("BuyOrder");
			break;
		case AT::BuySellType::SellOrder:
			return std::string("SellOrder");
			break;
		default:
			assert(false);
			return std::string("Assert False");
			break;
		}
	}

	std::string ToString( OrderType aType )
	{
		switch (aType)
		{
		case AT::OrderType::LimitOrder:
			return std::string("LimitOrder");
			break;
		case AT::OrderType::MarketOrder:
			return std::string("MarketOrder");
			break;
		default:
			assert(false);
			return std::string("Assert False");
			break;
		}
	}

	std::string ToString( TimeInForceType aType )
	{
		switch (aType)
		{
		case AT::TimeInForceType::IOC:
			return std::string("IOC");
			break;
		default:
			assert(false);
			return std::string("Assert False");
			break;
		}
	}

	std::string ToString( OpenCloseType aType )
	{
		switch (aType)
		{
		case AT::OpenCloseType::OpenOrder:
			return std::string("OpenOrder");
			break;
		case AT::OpenCloseType::CloseOrder:
			return std::string("CloseOrder");
			break;
		case AT::OpenCloseType::CloseTodayOrder:
			return std::string("CloseTodayOrder");
			break;
		default:
			assert(false);
			return std::string("Assert False");
			break;
		}
	}

	std::string ToString( OrderStatusType aType )
	{
		switch (aType)
		{
		case AT::OrderStatusType::RejectOrder:
			return std::string("RejectOrder");
			break;
		case AT::OrderStatusType::ActiveOrder:
			return std::string("ActiveOrder");
			break;
		case AT::OrderStatusType::StoppedOrder:
			return std::string("StoppedOrder");
			break;
		default:
			assert(false);
			return std::string("Assert False");
			break;
		}
	}

	std::string ToString( const AT_Time& aTime )
	{
		return boost::posix_time::to_simple_string( aTime.time_of_day());
	}

	std::string ToString( AT_Order_Key aKey )
	{
		return boost::uuids::to_string(aKey);
	}

}
	

