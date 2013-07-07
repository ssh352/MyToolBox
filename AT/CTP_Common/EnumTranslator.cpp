#include "EnumTranslator.h"

#include <boost\bimap.hpp>
using boost::bimap;
using boost::bimaps::set_of;
using namespace AT;
namespace CTP
{

	bimap< set_of< OrderPriceType >, set_of< char > >	m_OrderPriceTypeTrans;
	bimap< set_of< BuySellType >, set_of< char > >	m_BuySellTypeTrans;
	bimap< set_of< TriggerType >, set_of< char > >	m_TriggerTypeTrans;
	bimap< set_of< OpenCloseType >, set_of< char > >	m_OpenCloseTypeTrans;
	bimap< set_of< TimeInForceType >, set_of< char > >	m_TimeInForceTypeTrans;
	bimap< set_of< OrderStatusType >, set_of< char > >	m_OrderStatusTypeTrans;
	bimap< set_of< OrderSubmitStatusType >, set_of< char > >	m_OrderSubmitStatusTypeTrans;


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
		//{
		//	bimap< set_of< OrderPriceType >, set_of< char > >::value_type ValueType;
		//	m_OrderPriceTypeTrans.insert(ValueType())
		//}

	}

}
