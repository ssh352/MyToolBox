#pragma once
#include <boost\shared_ptr.hpp>

#include "OrderTable_CTP.h"
#include "TradeTable_CTP.h"
#include "InputOrderTable.h"
#include "PositionTable_CTP.h"
namespace CTP
{
	class DataCache_CTP_TD
	{
	public:
		DataCache_CTP_TD(void);
		~DataCache_CTP_TD(void);
	public:
		void UpdataOrder(boost::shared_ptr<CThostFtdcOrderField>  apOrder);
		void UpdataTrade(boost::shared_ptr<CThostFtdcTradeField> apTrade);
		void UpdataInputOrder(InputOrderTypePtr apInputOrder);

		void ClearPosition();
		void UpdatePosition(boost::shared_ptr<CThostFtdcInvestorPositionField> apPosition);
		std::string GeneratorPositionString();

		std::string GetThostOrderIDByExchangeOrderID(const std::string& lExchangeOrderID);
		std::string GetExchangeOrderIDByThostOrderID(const std::string& lThostOrderID);

		InputOrderTypePtr FindInputOrderByThostID(const std::string& lThostOrderID);
	private:
		std::string GeneratorOnePositionString(boost::shared_ptr<CThostFtdcInvestorPositionField>);
		std::string GetPositionID(boost::shared_ptr<CThostFtdcTradeField> apTrade);
		void UpdatePositionWithTrade(boost::shared_ptr<CThostFtdcTradeField> apTrade);
		void RebuildOrderIDMap();
		void UpdataOrderIDMap(boost::shared_ptr<CThostFtdcOrderField>  apOrder);
		std::map<std::string,std::string> m_ThostOrderID_ExchangOrderID;
		std::map<std::string,std::string> m_ExchangOrderID_ThostOrderID;
		OrderTable_CTP		m_OrderTable;
		TradeTable_CTP		m_TradeTable;
		InputOrderTable_CTP	m_InputOrderTable;
		PositionTable_CTP	m_PositionTable;
	};
}


