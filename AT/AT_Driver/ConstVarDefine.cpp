#include "ConstVarDefine.h"
#include <boost\uuid\uuid_io.hpp>
namespace AT
{

	extern const AT_Time AT_INVALID_TIME = boost::posix_time::ptime(boost::posix_time::not_a_date_time);




	struct EnumTextHelper
	{
		static std::map<OrderPriceType,std::string> m_OrderPriceType;
		static std::map<BuySellType,std::string> m_BuySellType;
		static std::map<TriggerType,std::string> m_TriggerType;
		static std::map<OpenCloseType,std::string> m_OpenCloseType;
		static std::map<TimeInForceType,std::string> m_TimeInForceType;
		static std::map<OrderStatusType,std::string> m_OrderStatusType;
		static std::map<OrderSubmitStatusType,std::string> m_OrderSubmitStatusType;

		static void Init()
		{
			m_OrderPriceType[OrderPriceType::NotSet] = "未设置价格类型";
			m_OrderPriceType[OrderPriceType::LimitPrice] = "限价";
			m_OrderPriceType[OrderPriceType::MarketPrice] = "市价";
			m_OrderPriceType[OrderPriceType::BestPrice] = "最优价";
			m_OrderPriceType[OrderPriceType::LastPrice] = "最新价";
			m_OrderPriceType[OrderPriceType::LastPricePlusOneTicks] = "最新价+1 tick";
			m_OrderPriceType[OrderPriceType::LastPricePlusTwoTicks] = "最新价+2 tick";
			m_OrderPriceType[OrderPriceType::LastPricePlusThreeTicks] = "最新价+3 tick";
			m_OrderPriceType[OrderPriceType::AskPrice1] = "卖一价";
			m_OrderPriceType[OrderPriceType::AskPrice1PlusOneTicks] = "卖一价+1 tick";
			m_OrderPriceType[OrderPriceType::AskPrice1PlusTwoTicks] = "卖一价+2 tick";
			m_OrderPriceType[OrderPriceType::AskPrice1PlusThreeTicks] = "卖一价+3 tick";
			m_OrderPriceType[OrderPriceType::BidPrice1] = "买一价";
			m_OrderPriceType[OrderPriceType::BidPrice1PlusOneTicks] = "买一价 +1 tick";
			m_OrderPriceType[OrderPriceType::BidPrice1PlusTwoTicks] = "买一价 +2 tick";
			m_OrderPriceType[OrderPriceType::BidPrice1PlusThreeTicks] = "买一价 +3 tick";


			m_BuySellType[BuySellType::NotSet] = "未设置";
			m_BuySellType[BuySellType::BuyOrder] = "买";
			m_BuySellType[BuySellType::SellOrder] = "卖";

			m_TriggerType[TriggerType::NotSet] = "未设置价格类型";
			m_TriggerType[TriggerType::Immediately] = "立即";
			m_TriggerType[TriggerType::Touch] = "止损";
			m_TriggerType[TriggerType::TouchProfit] = "止赢";
			m_TriggerType[TriggerType::ParkedOrder] = "预埋单";
			m_TriggerType[TriggerType::LastPriceGreaterThanStopPrice] = "最新价大于条件价";
			m_TriggerType[TriggerType::LastPriceGreaterEqualStopPrice] = "最新价大于等于条件价";
			m_TriggerType[TriggerType::LastPriceLesserThanStopPrice] = "最新价小于条件价";
			m_TriggerType[TriggerType::LastPriceLesserEqualStopPrice] = "最新价小于等于条件价";
			m_TriggerType[TriggerType::AskPriceGreaterThanStopPrice] = "卖一价大于条件价";
			m_TriggerType[TriggerType::AskPriceGreaterEqualStopPrice] = "卖一价大于等于条件价";
			m_TriggerType[TriggerType::AskPriceLesserThanStopPrice] = "卖一价小于条件价";
			m_TriggerType[TriggerType::AskPriceLesserEqualStopPrice] = "卖一价小于等于条件价";
			m_TriggerType[TriggerType::BidPriceGreaterThanStopPrice] = "买一价大于条件价";
			m_TriggerType[TriggerType::BidPriceGreaterEqualStopPrice] = "买一价大于等于条件价";
			m_TriggerType[TriggerType::BidPriceLesserThanStopPrice] = "买一价小于条件价";
			m_TriggerType[TriggerType::BidPriceLesserEqualStopPrice] = "买一价小于等于条件价";

			m_OpenCloseType[OpenCloseType::NotSet] = "未设置";
			m_OpenCloseType[OpenCloseType::Open] = "开仓";
			m_OpenCloseType[OpenCloseType::Close] = "平仓";
			m_OpenCloseType[OpenCloseType::ForceClose] = "强平";
			m_OpenCloseType[OpenCloseType::CloseToday] = "平今";
			m_OpenCloseType[OpenCloseType::CloseYesterday] = "平昨";
			m_OpenCloseType[OpenCloseType::ForceOff] = "强减";
			m_OpenCloseType[OpenCloseType::LocalForceClose] = "本地强平";


			m_TimeInForceType[TimeInForceType::NotSet] = "未设置";
			m_TimeInForceType[TimeInForceType::IOC] = "立即完成，否则撤销";
			m_TimeInForceType[TimeInForceType::GFS] = "本节有效";
			m_TimeInForceType[TimeInForceType::GFD] = "当日有效";
			m_TimeInForceType[TimeInForceType::GTD] = "指定日期前有效";
			m_TimeInForceType[TimeInForceType::GTC] = "撤销前有效";
			m_TimeInForceType[TimeInForceType::GFA] = "集合竞价有效";
				 
			 m_OrderStatusType[OrderStatusType::AllTraded] = "全部成交";
			 m_OrderStatusType[OrderStatusType::PartTradedQueueing] = "部分成交还在队列中";
			 m_OrderStatusType[OrderStatusType::PartTradedNotQueueing] = "部分成交不在队列中";
			 m_OrderStatusType[OrderStatusType::NoTradeQueueing] = "未成交还在队列中";
			 m_OrderStatusType[OrderStatusType::NoTradeNotQueueing] = "未成交不在队列中";
			 m_OrderStatusType[OrderStatusType::Canceled] = "撤单";
			 m_OrderStatusType[OrderStatusType::Unknown] = "未知";
			 m_OrderStatusType[OrderStatusType::NotTouched] = "尚未触发";
			 m_OrderStatusType[OrderStatusType::Touched] = "已触发";

			 m_OrderSubmitStatusType[OrderSubmitStatusType::InsertSubmitted] = "已经提交";
			 m_OrderSubmitStatusType[OrderSubmitStatusType::CancelSubmitted] = "撤单已经提交";
			 m_OrderSubmitStatusType[OrderSubmitStatusType::ModifySubmitted] = "修改已经提交";
			 m_OrderSubmitStatusType[OrderSubmitStatusType::Accepted] = "已经接受";
			 m_OrderSubmitStatusType[OrderSubmitStatusType::InsertRejected] = "报单已经被拒绝";
			 m_OrderSubmitStatusType[OrderSubmitStatusType::CancelRejected] = "撤单已经被拒绝";
			 m_OrderSubmitStatusType[OrderSubmitStatusType::ModifyRejected] = "改单已经被拒绝";
		}
	};

	 std::map<OrderPriceType,std::string> EnumTextHelper::m_OrderPriceType;
	 std::map<BuySellType,std::string>  EnumTextHelper::m_BuySellType;
	 std::map<TriggerType,std::string>  EnumTextHelper::m_TriggerType;
	 std::map<OpenCloseType,std::string>  EnumTextHelper::m_OpenCloseType;
	 std::map<TimeInForceType,std::string>  EnumTextHelper::m_TimeInForceType;
	 std::map<OrderStatusType,std::string>  EnumTextHelper::m_OrderStatusType;
	 std::map<OrderSubmitStatusType,std::string>  EnumTextHelper::m_OrderSubmitStatusType;

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
		return EnumTextHelper::m_BuySellType[aType];
	}

	std::string ToString( OrderPriceType aType )
	{
		return EnumTextHelper::m_OrderPriceType[aType];
	}

	std::string ToString( TimeInForceType aType )
	{
		return EnumTextHelper::m_TimeInForceType[aType];
	}

	std::string ToString( OpenCloseType aType )
	{
		return EnumTextHelper::m_OpenCloseType[aType];
	}

	std::string ToString( OrderStatusType aType )
	{
		return EnumTextHelper::m_OrderStatusType[aType];
	}

	std::string ToString( OrderSubmitStatusType aType )
	{
		return EnumTextHelper::m_OrderSubmitStatusType[aType];
	}

	std::string ToString( TriggerType aType )
	{
		return EnumTextHelper::m_TriggerType[aType];
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
	

