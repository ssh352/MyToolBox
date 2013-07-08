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
		NotSet,
		BuyOrder,
		SellOrder
	};
	std::string ToString(BuySellType aType);

	enum class OrderPriceType
	{
		NotSet,

		LimitPrice,	//限价

		MarketPrice,//市价

		
		BestPrice, //最优价
		
		LastPrice, ///最新价
		
		LastPricePlusOneTicks, ///最新价浮动上浮1个ticks
		
		LastPricePlusTwoTicks , ///最新价浮动上浮2个ticks
		
		LastPricePlusThreeTicks ,///最新价浮动上浮3个ticks
	
		AskPrice1 ,	///卖一价
		
		AskPrice1PlusOneTicks ,///卖一价浮动上浮1个ticks
		
		AskPrice1PlusTwoTicks ,///卖一价浮动上浮2个ticks
		
		AskPrice1PlusThreeTicks ,///卖一价浮动上浮3个ticks
		
		BidPrice1 ,///买一价
		
		BidPrice1PlusOneTicks ,///买一价浮动上浮1个ticks
		
		BidPrice1PlusTwoTicks ,///买一价浮动上浮2个ticks
		
		BidPrice1PlusThreeTicks ///买一价浮动上浮3个ticks
	};
	std::string ToString(OrderPriceType aType);


	enum class TimeInForceType
	{
		NotSet,
		
		IOC, //立即完成，否则撤销
		
		GFS , //本节有效
		
		GFD , //当日有效
		
		GTD, //指定日期前有效
		
		GTC, //撤销前有效
		
		GFA //集合竞价有效
	};
	std::string ToString(TimeInForceType aType);

	enum class OpenCloseType
	{
		NotSet,
	
		Open,	///开仓
		
		Close ,///平仓
	
		ForceClose,	///强平
	
		CloseToday ,	///平今
		
		CloseYesterday ,///平昨
		
		ForceOff ,///强减
		
		LocalForceClose///本地强平
	};
	std::string ToString(OpenCloseType aType);


	enum class TriggerType
	{
		NotSet,
		
		Immediately,///立即
		
		Touch,///止损
		
		TouchProfit ,///止赢
		
		ParkedOrder ,///预埋单
		
		LastPriceGreaterThanStopPrice ,///最新价大于条件价
		
		LastPriceGreaterEqualStopPrice ,///最新价大于等于条件价
		
		LastPriceLesserThanStopPrice,///最新价小于条件价
		
		LastPriceLesserEqualStopPrice ,///最新价小于等于条件价
	
		AskPriceGreaterThanStopPrice ,	///卖一价大于条件价
	
		AskPriceGreaterEqualStopPrice,	///卖一价大于等于条件价
		
		AskPriceLesserThanStopPrice,///卖一价小于条件价
		
		AskPriceLesserEqualStopPrice ,///卖一价小于等于条件价
	
		BidPriceGreaterThanStopPrice ,	///买一价大于条件价
		
		BidPriceGreaterEqualStopPrice,///买一价大于等于条件价
		
		BidPriceLesserThanStopPrice,///买一价小于条件价
		
		BidPriceLesserEqualStopPrice///买一价小于等于条件价
	};
	std::string ToString(TriggerType aType);


	enum class OrderSubmitStatusType
	{
	
		InsertSubmitted,	///已经提交
	
		CancelSubmitted,	///撤单已经提交
	
		ModifySubmitted ,	///修改已经提交
		
		Accepted,///已经接受
		
		InsertRejected,///报单已经被拒绝
	
		CancelRejected ,	///撤单已经被拒绝
	
		ModifyRejected 	///改单已经被拒绝
	};
	std::string ToString(OrderSubmitStatusType aType);

	enum class OrderStatusType
	{
		
		AllTraded ,///全部成交
		
		PartTradedQueueing,///部分成交还在队列中
		
		PartTradedNotQueueing ,///部分成交不在队列中
		
		NoTradeQueueing ,///未成交还在队列中
	
		NoTradeNotQueueing ,	///未成交不在队列中
		
		Canceled ,///撤单
	
		Unknown,	///未知
		
		NotTouched,///尚未触发
		
		Touched ///已触发
	};
	std::string ToString(OrderStatusType aType);

	typedef 	boost::uuids::uuid AT_Order_Key;
	std::string ToString(AT_Order_Key aKey);

	double TransPriceToDouble(int32_t aATPrice);
	int32_t TranPriceToInt(double aExchangePrice);



}

