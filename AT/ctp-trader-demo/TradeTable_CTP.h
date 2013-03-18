#pragma once
#include "ITradeTable.h"
#include <map>
#include <string>
#include <boost\thread.hpp>
struct CThostFtdcTradeField;

namespace CTP
{
	class TradeTable_CTP : public ITradeTable<CThostFtdcTradeField>
	{

	public:
		virtual void AddTrade( TradeTypePtr aTradePtr) ;
		virtual void TraverseAll(TradeTableTraver aTraver);

		//find by Product
		//find by user
		//find by Instrument
	private:
		std::map<std::string,TradeTypePtr> m_TradeMap;
		boost::mutex m_Lock;
	};
}



