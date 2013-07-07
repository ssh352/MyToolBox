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

		LimitPrice,

		MarketPrice,
		///���ż�
		BestPrice,
		///���¼�
		LastPrice,
		///���¼۸����ϸ�1��ticks
		LastPricePlusOneTicks,
		///���¼۸����ϸ�2��ticks
		LastPricePlusTwoTicks ,
		///���¼۸����ϸ�3��ticks
		LastPricePlusThreeTicks ,
		///��һ��
		AskPrice1 ,
		///��һ�۸����ϸ�1��ticks
		AskPrice1PlusOneTicks ,
		///��һ�۸����ϸ�2��ticks
		AskPrice1PlusTwoTicks ,
		///��һ�۸����ϸ�3��ticks
		_AskPrice1PlusThreeTicks ,
		///��һ��
		BidPrice1 ,
		///��һ�۸����ϸ�1��ticks
		BidPrice1PlusOneTicks ,
		///��һ�۸����ϸ�2��ticks
		BidPrice1PlusTwoTicks ,
		///��һ�۸����ϸ�3��ticks
		BidPrice1PlusThreeTicks 
	};
	std::string ToString(OrderPriceType aType);


	enum class TimeInForceType
	{
		NotSet,
		///������ɣ�������
		IOC,
		///������Ч
		GFS ,
		///������Ч
		GFD ,
		///ָ������ǰ��Ч
		GTD,
		///����ǰ��Ч
		GTC,
		///���Ͼ�����Ч
		GFA
	};
	std::string ToString(TimeInForceType aType);

	enum class OpenCloseType
	{
		NotSet,
		OpenOrder,
		CloseOrder,
		CloseTodayOrder
	};
	std::string ToString(OpenCloseType aType);


	enum class TriggerType
	{
		NotSet,
		///����
		Immediately,
		///ֹ��
		Touch,
		///ֹӮ
		TouchProfit ,
		///Ԥ��
		ParkedOrder ,
		///���¼۴���������
		LastPriceGreaterThanStopPrice ,
		///���¼۴��ڵ���������
		LastPriceGreaterEqualStopPrice ,
		///���¼�С��������
		LastPriceLesserThanStopPrice,
		///���¼�С�ڵ���������
		LastPriceLesserEqualStopPrice ,
		///��һ�۴���������
		AskPriceGreaterThanStopPrice ,
		///��һ�۴��ڵ���������
		AskPriceGreaterEqualStopPrice,
		///��һ��С��������
		AskPriceLesserThanStopPrice,
		///��һ��С�ڵ���������
		AskPriceLesserEqualStopPrice ,
		///��һ�۴���������
		BidPriceGreaterThanStopPrice ,
		///��һ�۴��ڵ���������
		BidPriceGreaterEqualStopPrice,
		///��һ��С��������
		BidPriceLesserThanStopPrice,
		///��һ��С�ڵ���������
		BidPriceLesserEqualStopPrice
	};
	std::string ToString(TriggerType aType);


	enum class OrderSubmitStatusType
	{
		///�Ѿ��ύ
		InsertSubmitted,
		///�����Ѿ��ύ
		CancelSubmitted,
		///�޸��Ѿ��ύ
		ModifySubmitted ,
		///�Ѿ�����
		Accepted,
		///�����Ѿ����ܾ�
		InsertRejected,
		///�����Ѿ����ܾ�
		CancelRejected ,
		///�ĵ��Ѿ����ܾ�
		ModifyRejected 
	};
	std::string ToString(OrderSubmitStatusType aType);

	enum class OrderStatusType
	{
		///ȫ���ɽ�
		AllTraded ,
		///���ֳɽ����ڶ�����
		PartTradedQueueing,
		///���ֳɽ����ڶ�����
		PartTradedNotQueueing ,
		///δ�ɽ����ڶ�����
		NoTradeQueueing ,
		///δ�ɽ����ڶ�����
		NoTradeNotQueueing ,
		///����
		Canceled ,
		///δ֪
		Unknown,
		///��δ����
		NotTouched,
		///�Ѵ���
		Touched 
	};
	std::string ToString(OrderStatusType aType);

	typedef 	boost::uuids::uuid AT_Order_Key;
	std::string ToString(AT_Order_Key aKey);

	double TransPriceToDouble(int32_t aATPrice);
	int32_t TranPriceToInt(double aExchangePrice);



}

