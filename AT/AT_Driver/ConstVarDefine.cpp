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

	std::string ToString( OrderPriceType aType )
	{
		//switch (aType)
		//{
		//case AT::OrderPriceType::LimitOrder:
		//	return std::string("LimitOrder");
		//	break;
		//case AT::OrderPriceType::MarketOrder:
		//	return std::string("MarketOrder");
		//	break;
		//case AT::OrderPriceType::StopLossOrder:
		//	return std::string("StopLossOrder");
		//	break;
		//default:
		//	assert(false);
		//	return std::string("Assert False");
		//	break;
		//}
		return std::string("PriceType todo");
	}

	std::string ToString( TimeInForceType aType )
	{
		switch (aType)
		{
		case AT::TimeInForceType::IOC:
			return std::string("IOC");
			break;
		case AT::TimeInForceType::GOOD_FOR_DAY:
			return std::string("GOOD_FOR_DAY");
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
		case  AT::OrderStatusType::UnHandleOrder:
			return std::string("UnHandleOrder");
			break;
		case OrderStatusType::SuspendOrder:
			return  std::string("SuspendOrder");
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

	std::string ToString( OrderSubmitStatusType aType )
	{
		return std::string();
	}

	std::string ToString( TriggerType aType )
	{
		return std::string();
	}

}
	

