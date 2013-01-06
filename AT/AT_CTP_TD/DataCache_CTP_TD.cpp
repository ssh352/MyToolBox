#include "DataCache_CTP_TD.h"
#include <boost/foreach.hpp>
namespace CTP
{
	DataCache_CTP_TD::DataCache_CTP_TD(void)
	{
		m_OrderTable.InitWithDB("OrderMapDB");
		m_TradeTable.InitWithDB("TradeMapDB");
		RebuildOrderIDMap();
	}

	void DataCache_CTP_TD::RebuildOrderIDMap()
	{
		//highLight ����������� �����������û��foreach �� auto ����Ҫд���ٴ��������������߼�ô��
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

		if(m_ThostOrderID_ExchangOrderID.count(lThostOrderID))
		{
			if(m_ThostOrderID_ExchangOrderID[lThostOrderID] != lExchangeOrderID)
				throw std::logic_error("OrderID Map rechange");
		}
		m_ThostOrderID_ExchangOrderID[lThostOrderID] = lExchangeOrderID;
		m_ExchangOrderID_ThostOrderID[lExchangeOrderID] = lThostOrderID;
	}


}

