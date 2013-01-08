#pragma once

#include <map>
#include <boost/shared_ptr.hpp>
#include "ItemTable.h"
#include "CTP_API.h"
namespace CTP
{
	typedef boost::shared_ptr<CThostFtdcInvestorPositionField> Postion_Ptr;

	class CThostFtdcInvestorPositionFieldTraits
	{
	public:
		static std::string GetItemID(Postion_Ptr apTrade)
		{
			std::string lRet;
			return lRet;
		}
		static std::string GetItemTypeName(){return "CThostFtdcInvestorPositionField";};
		static const int IsDuplicateException = 1;
	};


	class PositionTable_CTP :public ItemTable<CThostFtdcInvestorPositionField,CThostFtdcInvestorPositionFieldTraits>
	{
	public:
		//void AddTrade(Trade_Ptr apOrder );
		//Trade_Ptr GetTrade(const std::string& aTradeID);
	};
}


