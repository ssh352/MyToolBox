#include "DataCache_CTP_TD.h"
#include <boost/foreach.hpp>
namespace CTP
{
	DataCache_CTP_TD::DataCache_CTP_TD(void)
	{
		m_OrderTable.InitWithDB("OrderMapDB");
		m_TradeTable.InitWithDB("TradeMapDB");
		m_InputOrderTable.InitWithDB("InputOrderMapDB");
		RebuildOrderIDMap();
	}

	void DataCache_CTP_TD::RebuildOrderIDMap()
	{
		//highLight 生活如此美好 你能想象如果没有foreach 和 auto 我需要写多少代码来完成这个简单逻辑么？
		BOOST_FOREACH(  auto lPair, m_OrderTable)
		{
			UpdataOrderIDMap(lPair.second);
		}
	}

	DataCache_CTP_TD::~DataCache_CTP_TD(void)
	{
	}

	void DataCache_CTP_TD::UpdataOrder( boost::shared_ptr<CThostFtdcOrderField> apOrder )
	{
		m_OrderTable.AddOrder(apOrder);
		UpdataOrderIDMap(apOrder);
	}

	void DataCache_CTP_TD::UpdataTrade( boost::shared_ptr<CThostFtdcTradeField> apTrade )
	{
		m_TradeTable.AddTrade(apTrade);
	}

	void DataCache_CTP_TD::UpdataOrderIDMap( boost::shared_ptr<CThostFtdcOrderField> apOrder )
	{
		std::string lExchangeOrderID = GenerateExchangeOrderID(apOrder);
		std::string lThostOrderID  = GenerateThostOrderID(apOrder);
		m_ThostOrderID_ExchangOrderID[lThostOrderID] = lExchangeOrderID;
		m_ExchangOrderID_ThostOrderID[lExchangeOrderID] = lThostOrderID;
	}

	std::string DataCache_CTP_TD::GetThostOrderIDByExchangeOrderID( const std::string& lExchangeOrderID )
	{
		if(m_ExchangOrderID_ThostOrderID.count(lExchangeOrderID))
		{
			return m_ExchangOrderID_ThostOrderID[lExchangeOrderID];
		}
		else
		{
			throw std::logic_error("Can not GetThostOrderIDByExchangeOrderID " + lExchangeOrderID);
		}
	}

	std::string DataCache_CTP_TD::GetExchangeOrderIDByThostOrderID( const std::string& lThostOrderID )
	{
		if(m_ThostOrderID_ExchangOrderID.count(lThostOrderID))
		{
			return m_ThostOrderID_ExchangOrderID[lThostOrderID];
		}
		else
		{
			throw std::logic_error("Can not GetExchangeOrderIDByThostOrderID  "  + lThostOrderID );
		}
	}

	void DataCache_CTP_TD::UpdataInputOrder( InputOrderTypePtr apInputOrder )
	{
		m_InputOrderTable.AddOrder(apInputOrder);
	}

	InputOrderTypePtr DataCache_CTP_TD::FindInputOrderByThostID( const std::string& lThostOrderID )
	{
		return	m_InputOrderTable.GetOrder(lThostOrderID);
	}


}

