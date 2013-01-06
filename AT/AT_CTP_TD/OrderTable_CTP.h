#pragma once

#include <map>
#include <boost/shared_ptr.hpp>
#include "ItemTable.h"
#include "CTP_API.h"
namespace CTP
{
	typedef boost::shared_ptr<CThostFtdcOrderField> Order_Ptr;

	class CThostFtdcOrderFieldTraits
	{
	public:
		static std::string GetItemID(Order_Ptr apOrder)
		{
			return GenerateThostOrderID(apOrder);
		}
		static std::string GetItemTypeName(){return "CThostFtdcOrderField";};
		static const int IsDuplicateException = 0;
	};


	class OrderTable_CTP :public ItemTable<CThostFtdcOrderField,CThostFtdcOrderFieldTraits>
	{
	public:
		void AddOrder(Order_Ptr apOrder );
		Order_Ptr GetOrder(const std::string& aThostOrderID);
	};
}


