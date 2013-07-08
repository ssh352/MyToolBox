#include "EnumTranslator.h"

#include <boost\bimap.hpp>

#include "ThostFtdcUserApiStruct.h"
using boost::bimap;
using boost::bimaps::set_of;
using namespace AT;
namespace CTP
{

	bimap<  OrderPriceType, char  >	m_OrderPriceTypeTrans;
	bimap<  BuySellType , char > 	m_BuySellTypeTrans;
	bimap<  TriggerType ,char > 	m_TriggerTypeTrans;
	bimap<  OpenCloseType ,  char >	m_OpenCloseTypeTrans;
	bimap< TimeInForceType, char > 	m_TimeInForceTypeTrans;
	bimap< OrderStatusType ,  char >	m_OrderStatusTypeTrans;
	bimap< OrderSubmitStatusType ,  char >	m_OrderSubmitStatusTypeTrans;


	char EnumTranslator::TransOrderPriceType(AT::OrderPriceType  aVal){ return m_OrderPriceTypeTrans.left.at(aVal);}
	AT::OrderPriceType	EnumTranslator::TransOrderPriceType(char aVal){ return m_OrderPriceTypeTrans.right.at(aVal);}

	char	EnumTranslator::TransBuySellType(AT::BuySellType aVal){ return m_BuySellTypeTrans.left.at(aVal);}
	AT::BuySellType		EnumTranslator::TransBuySellType(char aVal){ return m_BuySellTypeTrans.right.at(aVal);}

	char	EnumTranslator::TransTriggerType(AT::TriggerType aVal){ return m_TriggerTypeTrans.left.at(aVal);}
	AT::TriggerType EnumTranslator::TransTriggerType(char aVal){ return m_TriggerTypeTrans.right.at(aVal);}

	char	EnumTranslator::TransOpenCloseType(AT::OpenCloseType aVal){ return m_OpenCloseTypeTrans.left.at(aVal);}
	AT::OpenCloseType	EnumTranslator::TransOpenCloseType(char aVal){ return m_OpenCloseTypeTrans.right.at(aVal);}

	char	EnumTranslator::TransTimeType(AT::TimeInForceType aVal){ return m_TimeInForceTypeTrans.left.at(aVal);}
	AT::TimeInForceType EnumTranslator::TransTimeType(char aVal){ return m_TimeInForceTypeTrans.right.at(aVal);}

	char EnumTranslator::TransOrderStatusType(AT::OrderStatusType aVal){ return m_OrderStatusTypeTrans.left.at(aVal);}
	AT::OrderStatusType	EnumTranslator::TransOrderStatusType(char aVal){ return m_OrderStatusTypeTrans.right.at(aVal);}

	char EnumTranslator::TransOrderSubmitStatusType(AT::OrderSubmitStatusType aVal){ return m_OrderSubmitStatusTypeTrans.left.at(aVal);}
	AT::OrderSubmitStatusType EnumTranslator::TransOrderSubmitStatusType(char aVal){ return m_OrderSubmitStatusTypeTrans.right.at(aVal);}


	void EnumTranslator::Init()
	{
		{
			typedef bimap<  OrderPriceType ,  char  >::value_type ValueType;
			m_OrderPriceTypeTrans.insert(ValueType(AT::OrderPriceType::NotSet,0));
			m_OrderPriceTypeTrans.insert(ValueType(AT::OrderPriceType::LimitPrice,THOST_FTDC_OPT_LimitPrice));
			m_OrderPriceTypeTrans.insert(ValueType(AT::OrderPriceType::MarketPrice,THOST_FTDC_OPT_AnyPrice));
			m_OrderPriceTypeTrans.insert(ValueType(AT::OrderPriceType::BestPrice,THOST_FTDC_OPT_BestPrice));
			m_OrderPriceTypeTrans.insert(ValueType(AT::OrderPriceType::LastPrice,THOST_FTDC_OPT_LastPrice));
			m_OrderPriceTypeTrans.insert(ValueType(AT::OrderPriceType::LastPricePlusOneTicks,THOST_FTDC_OPT_LastPricePlusOneTicks));
			m_OrderPriceTypeTrans.insert(ValueType(AT::OrderPriceType::LastPricePlusTwoTicks,THOST_FTDC_OPT_LastPricePlusTwoTicks));
			m_OrderPriceTypeTrans.insert(ValueType(AT::OrderPriceType::LastPricePlusThreeTicks,THOST_FTDC_OPT_LastPricePlusThreeTicks));
			m_OrderPriceTypeTrans.insert(ValueType(AT::OrderPriceType::AskPrice1,THOST_FTDC_OPT_AskPrice1));
			m_OrderPriceTypeTrans.insert(ValueType(AT::OrderPriceType::AskPrice1PlusOneTicks,THOST_FTDC_OPT_AskPrice1PlusOneTicks));
			m_OrderPriceTypeTrans.insert(ValueType(AT::OrderPriceType::AskPrice1PlusTwoTicks,THOST_FTDC_OPT_AskPrice1PlusTwoTicks));
			m_OrderPriceTypeTrans.insert(ValueType(AT::OrderPriceType::AskPrice1PlusThreeTicks,THOST_FTDC_OPT_AskPrice1PlusThreeTicks));
			m_OrderPriceTypeTrans.insert(ValueType(AT::OrderPriceType::BidPrice1,THOST_FTDC_OPT_BidPrice1));
			m_OrderPriceTypeTrans.insert(ValueType(AT::OrderPriceType::BidPrice1PlusOneTicks,THOST_FTDC_OPT_BidPrice1PlusOneTicks));
			m_OrderPriceTypeTrans.insert(ValueType(AT::OrderPriceType::BidPrice1PlusTwoTicks,THOST_FTDC_OPT_BidPrice1PlusTwoTicks));
			m_OrderPriceTypeTrans.insert(ValueType(AT::OrderPriceType::BidPrice1PlusThreeTicks,THOST_FTDC_OPT_BidPrice1PlusThreeTicks));
		}

		{
			typedef bimap<  BuySellType ,  char  >::value_type ValueType;
			m_BuySellTypeTrans.insert(ValueType(AT::BuySellType::NotSet,0));
			m_BuySellTypeTrans.insert(ValueType(AT::BuySellType::BuyOrder,THOST_FTDC_D_Buy));
			m_BuySellTypeTrans.insert(ValueType(AT::BuySellType::SellOrder,THOST_FTDC_D_Sell));
		}


		{
			typedef bimap<  TriggerType ,  char  >::value_type ValueType;
			m_TriggerTypeTrans.insert(ValueType(AT::TriggerType::NotSet,0));
			m_TriggerTypeTrans.insert(ValueType(AT::TriggerType::Immediately,THOST_FTDC_CC_Immediately));
			m_TriggerTypeTrans.insert(ValueType(AT::TriggerType::Touch,THOST_FTDC_CC_Touch));
			m_TriggerTypeTrans.insert(ValueType(AT::TriggerType::TouchProfit,THOST_FTDC_CC_TouchProfit));
			m_TriggerTypeTrans.insert(ValueType(AT::TriggerType::ParkedOrder,THOST_FTDC_CC_ParkedOrder));
			m_TriggerTypeTrans.insert(ValueType(AT::TriggerType::LastPriceGreaterThanStopPrice,THOST_FTDC_CC_LastPriceGreaterThanStopPrice));
			m_TriggerTypeTrans.insert(ValueType(AT::TriggerType::LastPriceGreaterEqualStopPrice,THOST_FTDC_CC_LastPriceGreaterEqualStopPrice));
			m_TriggerTypeTrans.insert(ValueType(AT::TriggerType::LastPriceLesserThanStopPrice,THOST_FTDC_CC_LastPriceLesserThanStopPrice));
			m_TriggerTypeTrans.insert(ValueType(AT::TriggerType::LastPriceLesserEqualStopPrice,THOST_FTDC_CC_LastPriceLesserEqualStopPrice));
			m_TriggerTypeTrans.insert(ValueType(AT::TriggerType::AskPriceGreaterThanStopPrice,THOST_FTDC_CC_AskPriceGreaterThanStopPrice));
			m_TriggerTypeTrans.insert(ValueType(AT::TriggerType::AskPriceGreaterEqualStopPrice,THOST_FTDC_CC_AskPriceGreaterEqualStopPrice));
			m_TriggerTypeTrans.insert(ValueType(AT::TriggerType::AskPriceLesserThanStopPrice,THOST_FTDC_CC_AskPriceLesserThanStopPrice));
			m_TriggerTypeTrans.insert(ValueType(AT::TriggerType::AskPriceLesserEqualStopPrice,THOST_FTDC_CC_AskPriceLesserEqualStopPrice));
			m_TriggerTypeTrans.insert(ValueType(AT::TriggerType::BidPriceGreaterThanStopPrice,THOST_FTDC_CC_BidPriceGreaterThanStopPrice));
			m_TriggerTypeTrans.insert(ValueType(AT::TriggerType::BidPriceGreaterEqualStopPrice,THOST_FTDC_CC_BidPriceGreaterEqualStopPrice));
			m_TriggerTypeTrans.insert(ValueType(AT::TriggerType::BidPriceLesserThanStopPrice,THOST_FTDC_CC_BidPriceLesserThanStopPrice));
			m_TriggerTypeTrans.insert(ValueType(AT::TriggerType::BidPriceLesserEqualStopPrice,THOST_FTDC_CC_BidPriceLesserEqualStopPrice));
		}

		{
			typedef bimap<  OpenCloseType ,  char  >::value_type ValueType;
			m_OpenCloseTypeTrans.insert(ValueType(AT::OpenCloseType::NotSet,0));
			m_OpenCloseTypeTrans.insert(ValueType(AT::OpenCloseType::CloseToday,THOST_FTDC_OF_Open));
			m_OpenCloseTypeTrans.insert(ValueType(AT::OpenCloseType::Close,THOST_FTDC_OF_Close));
			m_OpenCloseTypeTrans.insert(ValueType(AT::OpenCloseType::ForceClose,THOST_FTDC_OF_ForceClose));
			m_OpenCloseTypeTrans.insert(ValueType(AT::OpenCloseType::CloseToday,THOST_FTDC_OF_CloseToday));
			m_OpenCloseTypeTrans.insert(ValueType(AT::OpenCloseType::CloseYesterday,THOST_FTDC_OF_CloseYesterday));
			m_OpenCloseTypeTrans.insert(ValueType(AT::OpenCloseType::ForceOff,THOST_FTDC_OF_ForceOff));
			m_OpenCloseTypeTrans.insert(ValueType(AT::OpenCloseType::LocalForceClose,THOST_FTDC_OF_LocalForceClose));
		}
		{
			typedef bimap<  TimeInForceType ,  char  >::value_type ValueType;
			m_TimeInForceTypeTrans.insert(ValueType(AT::TimeInForceType::NotSet,THOST_FTDC_TC_GFD));
			m_TimeInForceTypeTrans.insert(ValueType(AT::TimeInForceType::IOC,THOST_FTDC_TC_IOC));
			m_TimeInForceTypeTrans.insert(ValueType(AT::TimeInForceType::GFS,THOST_FTDC_TC_GFS));
			m_TimeInForceTypeTrans.insert(ValueType(AT::TimeInForceType::GFD,THOST_FTDC_TC_GFD));
			m_TimeInForceTypeTrans.insert(ValueType(AT::TimeInForceType::GTD,THOST_FTDC_TC_GTD));
			m_TimeInForceTypeTrans.insert(ValueType(AT::TimeInForceType::GTC,THOST_FTDC_TC_GTC));
			m_TimeInForceTypeTrans.insert(ValueType(AT::TimeInForceType::GFA,THOST_FTDC_TC_GFA));
		}

		{			
			typedef bimap<  OrderStatusType ,  char  >::value_type ValueType;
			m_OrderStatusTypeTrans.insert(ValueType(AT::OrderStatusType::AllTraded,THOST_FTDC_OST_AllTraded));
			m_OrderStatusTypeTrans.insert(ValueType(AT::OrderStatusType::PartTradedQueueing,THOST_FTDC_OST_PartTradedQueueing));
			m_OrderStatusTypeTrans.insert(ValueType(AT::OrderStatusType::PartTradedNotQueueing,THOST_FTDC_OST_PartTradedNotQueueing));
			m_OrderStatusTypeTrans.insert(ValueType(AT::OrderStatusType::NoTradeQueueing,THOST_FTDC_OST_NoTradeQueueing));
			m_OrderStatusTypeTrans.insert(ValueType(AT::OrderStatusType::NoTradeNotQueueing,THOST_FTDC_OST_NoTradeNotQueueing));
			m_OrderStatusTypeTrans.insert(ValueType(AT::OrderStatusType::Canceled,THOST_FTDC_OST_Canceled));
			m_OrderStatusTypeTrans.insert(ValueType(AT::OrderStatusType::Unknown,THOST_FTDC_OST_Unknown));
			m_OrderStatusTypeTrans.insert(ValueType(AT::OrderStatusType::NotTouched,THOST_FTDC_OST_NotTouched));
			m_OrderStatusTypeTrans.insert(ValueType(AT::OrderStatusType::Touched,THOST_FTDC_OST_Touched));
		}

		{			

			typedef bimap<  OrderSubmitStatusType ,  char  >::value_type ValueType;
			m_OrderSubmitStatusTypeTrans.insert(ValueType(AT::OrderSubmitStatusType::InsertSubmitted,THOST_FTDC_OSS_InsertSubmitted));
			m_OrderSubmitStatusTypeTrans.insert(ValueType(AT::OrderSubmitStatusType::CancelSubmitted,THOST_FTDC_OSS_CancelSubmitted));
			m_OrderSubmitStatusTypeTrans.insert(ValueType(AT::OrderSubmitStatusType::ModifySubmitted,THOST_FTDC_OSS_ModifySubmitted));
			m_OrderSubmitStatusTypeTrans.insert(ValueType(AT::OrderSubmitStatusType::Accepted,THOST_FTDC_OSS_Accepted));
			m_OrderSubmitStatusTypeTrans.insert(ValueType(AT::OrderSubmitStatusType::InsertRejected,THOST_FTDC_OSS_InsertRejected));
			m_OrderSubmitStatusTypeTrans.insert(ValueType(AT::OrderSubmitStatusType::CancelRejected,THOST_FTDC_OSS_CancelRejected));
			m_OrderSubmitStatusTypeTrans.insert(ValueType(AT::OrderSubmitStatusType::ModifyRejected,THOST_FTDC_OSS_ModifyRejected));

		}
	}


}
