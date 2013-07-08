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
			m_OrderPriceType[OrderPriceType::NotSet] = "δ���ü۸�����";
			m_OrderPriceType[OrderPriceType::LimitPrice] = "�޼�";
			m_OrderPriceType[OrderPriceType::MarketPrice] = "�м�";
			m_OrderPriceType[OrderPriceType::BestPrice] = "���ż�";
			m_OrderPriceType[OrderPriceType::LastPrice] = "���¼�";
			m_OrderPriceType[OrderPriceType::LastPricePlusOneTicks] = "���¼�+1 tick";
			m_OrderPriceType[OrderPriceType::LastPricePlusTwoTicks] = "���¼�+2 tick";
			m_OrderPriceType[OrderPriceType::LastPricePlusThreeTicks] = "���¼�+3 tick";
			m_OrderPriceType[OrderPriceType::AskPrice1] = "��һ��";
			m_OrderPriceType[OrderPriceType::AskPrice1PlusOneTicks] = "��һ��+1 tick";
			m_OrderPriceType[OrderPriceType::AskPrice1PlusTwoTicks] = "��һ��+2 tick";
			m_OrderPriceType[OrderPriceType::AskPrice1PlusThreeTicks] = "��һ��+3 tick";
			m_OrderPriceType[OrderPriceType::BidPrice1] = "��һ��";
			m_OrderPriceType[OrderPriceType::BidPrice1PlusOneTicks] = "��һ�� +1 tick";
			m_OrderPriceType[OrderPriceType::BidPrice1PlusTwoTicks] = "��һ�� +2 tick";
			m_OrderPriceType[OrderPriceType::BidPrice1PlusThreeTicks] = "��һ�� +3 tick";


			m_BuySellType[BuySellType::NotSet] = "δ����";
			m_BuySellType[BuySellType::BuyOrder] = "��";
			m_BuySellType[BuySellType::SellOrder] = "��";

			m_TriggerType[TriggerType::NotSet] = "δ���ü۸�����";
			m_TriggerType[TriggerType::Immediately] = "����";
			m_TriggerType[TriggerType::Touch] = "ֹ��";
			m_TriggerType[TriggerType::TouchProfit] = "ֹӮ";
			m_TriggerType[TriggerType::ParkedOrder] = "Ԥ��";
			m_TriggerType[TriggerType::LastPriceGreaterThanStopPrice] = "���¼۴���������";
			m_TriggerType[TriggerType::LastPriceGreaterEqualStopPrice] = "���¼۴��ڵ���������";
			m_TriggerType[TriggerType::LastPriceLesserThanStopPrice] = "���¼�С��������";
			m_TriggerType[TriggerType::LastPriceLesserEqualStopPrice] = "���¼�С�ڵ���������";
			m_TriggerType[TriggerType::AskPriceGreaterThanStopPrice] = "��һ�۴���������";
			m_TriggerType[TriggerType::AskPriceGreaterEqualStopPrice] = "��һ�۴��ڵ���������";
			m_TriggerType[TriggerType::AskPriceLesserThanStopPrice] = "��һ��С��������";
			m_TriggerType[TriggerType::AskPriceLesserEqualStopPrice] = "��һ��С�ڵ���������";
			m_TriggerType[TriggerType::BidPriceGreaterThanStopPrice] = "��һ�۴���������";
			m_TriggerType[TriggerType::BidPriceGreaterEqualStopPrice] = "��һ�۴��ڵ���������";
			m_TriggerType[TriggerType::BidPriceLesserThanStopPrice] = "��һ��С��������";
			m_TriggerType[TriggerType::BidPriceLesserEqualStopPrice] = "��һ��С�ڵ���������";

			m_OpenCloseType[OpenCloseType::NotSet] = "δ����";
			m_OpenCloseType[OpenCloseType::Open] = "����";
			m_OpenCloseType[OpenCloseType::Close] = "ƽ��";
			m_OpenCloseType[OpenCloseType::ForceClose] = "ǿƽ";
			m_OpenCloseType[OpenCloseType::CloseToday] = "ƽ��";
			m_OpenCloseType[OpenCloseType::CloseYesterday] = "ƽ��";
			m_OpenCloseType[OpenCloseType::ForceOff] = "ǿ��";
			m_OpenCloseType[OpenCloseType::LocalForceClose] = "����ǿƽ";


			m_TimeInForceType[TimeInForceType::NotSet] = "δ����";
			m_TimeInForceType[TimeInForceType::IOC] = "������ɣ�������";
			m_TimeInForceType[TimeInForceType::GFS] = "������Ч";
			m_TimeInForceType[TimeInForceType::GFD] = "������Ч";
			m_TimeInForceType[TimeInForceType::GTD] = "ָ������ǰ��Ч";
			m_TimeInForceType[TimeInForceType::GTC] = "����ǰ��Ч";
			m_TimeInForceType[TimeInForceType::GFA] = "���Ͼ�����Ч";
				 
			 m_OrderStatusType[OrderStatusType::AllTraded] = "ȫ���ɽ�";
			 m_OrderStatusType[OrderStatusType::PartTradedQueueing] = "���ֳɽ����ڶ�����";
			 m_OrderStatusType[OrderStatusType::PartTradedNotQueueing] = "���ֳɽ����ڶ�����";
			 m_OrderStatusType[OrderStatusType::NoTradeQueueing] = "δ�ɽ����ڶ�����";
			 m_OrderStatusType[OrderStatusType::NoTradeNotQueueing] = "δ�ɽ����ڶ�����";
			 m_OrderStatusType[OrderStatusType::Canceled] = "����";
			 m_OrderStatusType[OrderStatusType::Unknown] = "δ֪";
			 m_OrderStatusType[OrderStatusType::NotTouched] = "��δ����";
			 m_OrderStatusType[OrderStatusType::Touched] = "�Ѵ���";

			 m_OrderSubmitStatusType[OrderSubmitStatusType::InsertSubmitted] = "�Ѿ��ύ";
			 m_OrderSubmitStatusType[OrderSubmitStatusType::CancelSubmitted] = "�����Ѿ��ύ";
			 m_OrderSubmitStatusType[OrderSubmitStatusType::ModifySubmitted] = "�޸��Ѿ��ύ";
			 m_OrderSubmitStatusType[OrderSubmitStatusType::Accepted] = "�Ѿ�����";
			 m_OrderSubmitStatusType[OrderSubmitStatusType::InsertRejected] = "�����Ѿ����ܾ�";
			 m_OrderSubmitStatusType[OrderSubmitStatusType::CancelRejected] = "�����Ѿ����ܾ�";
			 m_OrderSubmitStatusType[OrderSubmitStatusType::ModifyRejected] = "�ĵ��Ѿ����ܾ�";
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
	

