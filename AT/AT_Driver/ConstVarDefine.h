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

		LimitPrice,	//�޼�

		MarketPrice,//�м�

		
		BestPrice, //���ż�
		
		LastPrice, ///���¼�
		
		LastPricePlusOneTicks, ///���¼۸����ϸ�1��ticks
		
		LastPricePlusTwoTicks , ///���¼۸����ϸ�2��ticks
		
		LastPricePlusThreeTicks ,///���¼۸����ϸ�3��ticks
	
		AskPrice1 ,	///��һ��
		
		AskPrice1PlusOneTicks ,///��һ�۸����ϸ�1��ticks
		
		AskPrice1PlusTwoTicks ,///��һ�۸����ϸ�2��ticks
		
		AskPrice1PlusThreeTicks ,///��һ�۸����ϸ�3��ticks
		
		BidPrice1 ,///��һ��
		
		BidPrice1PlusOneTicks ,///��һ�۸����ϸ�1��ticks
		
		BidPrice1PlusTwoTicks ,///��һ�۸����ϸ�2��ticks
		
		BidPrice1PlusThreeTicks ///��һ�۸����ϸ�3��ticks
	};
	std::string ToString(OrderPriceType aType);


	enum class TimeInForceType
	{
		NotSet,
		
		IOC, //������ɣ�������
		
		GFS , //������Ч
		
		GFD , //������Ч
		
		GTD, //ָ������ǰ��Ч
		
		GTC, //����ǰ��Ч
		
		GFA //���Ͼ�����Ч
	};
	std::string ToString(TimeInForceType aType);

	enum class OpenCloseType
	{
		NotSet,
	
		Open,	///����
		
		Close ,///ƽ��
	
		ForceClose,	///ǿƽ
	
		CloseToday ,	///ƽ��
		
		CloseYesterday ,///ƽ��
		
		ForceOff ,///ǿ��
		
		LocalForceClose///����ǿƽ
	};
	std::string ToString(OpenCloseType aType);


	enum class TriggerType
	{
		NotSet,
		
		Immediately,///����
		
		Touch,///ֹ��
		
		TouchProfit ,///ֹӮ
		
		ParkedOrder ,///Ԥ��
		
		LastPriceGreaterThanStopPrice ,///���¼۴���������
		
		LastPriceGreaterEqualStopPrice ,///���¼۴��ڵ���������
		
		LastPriceLesserThanStopPrice,///���¼�С��������
		
		LastPriceLesserEqualStopPrice ,///���¼�С�ڵ���������
	
		AskPriceGreaterThanStopPrice ,	///��һ�۴���������
	
		AskPriceGreaterEqualStopPrice,	///��һ�۴��ڵ���������
		
		AskPriceLesserThanStopPrice,///��һ��С��������
		
		AskPriceLesserEqualStopPrice ,///��һ��С�ڵ���������
	
		BidPriceGreaterThanStopPrice ,	///��һ�۴���������
		
		BidPriceGreaterEqualStopPrice,///��һ�۴��ڵ���������
		
		BidPriceLesserThanStopPrice,///��һ��С��������
		
		BidPriceLesserEqualStopPrice///��һ��С�ڵ���������
	};
	std::string ToString(TriggerType aType);


	enum class OrderSubmitStatusType
	{
	
		InsertSubmitted,	///�Ѿ��ύ
	
		CancelSubmitted,	///�����Ѿ��ύ
	
		ModifySubmitted ,	///�޸��Ѿ��ύ
		
		Accepted,///�Ѿ�����
		
		InsertRejected,///�����Ѿ����ܾ�
	
		CancelRejected ,	///�����Ѿ����ܾ�
	
		ModifyRejected 	///�ĵ��Ѿ����ܾ�
	};
	std::string ToString(OrderSubmitStatusType aType);

	enum class OrderStatusType
	{
		
		AllTraded ,///ȫ���ɽ�
		
		PartTradedQueueing,///���ֳɽ����ڶ�����
		
		PartTradedNotQueueing ,///���ֳɽ����ڶ�����
		
		NoTradeQueueing ,///δ�ɽ����ڶ�����
	
		NoTradeNotQueueing ,	///δ�ɽ����ڶ�����
		
		Canceled ,///����
	
		Unknown,	///δ֪
		
		NotTouched,///��δ����
		
		Touched ///�Ѵ���
	};
	std::string ToString(OrderStatusType aType);

	typedef 	boost::uuids::uuid AT_Order_Key;
	std::string ToString(AT_Order_Key aKey);

	double TransPriceToDouble(int32_t aATPrice);
	int32_t TranPriceToInt(double aExchangePrice);



}

