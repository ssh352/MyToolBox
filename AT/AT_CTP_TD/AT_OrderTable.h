#pragma once
#include "ItemTable.h"
#include "AT_Struct.h"

namespace AT
{

	class OrderUpdateTraits
	{
		static std::string GetItemID(InputOrderTypePtr apOrder)
		{
			
		}
	};

	class AT_OrderTable :public ItemTable<AT::OrderUpdate,CThostFtdcOrderFieldTraits>
	{
	public:
		void AddOrder(Order_Ptr apOrder );
		Order_Ptr GetOrder(const std::string& aThostOrderID);
	};

	class AT_OrderTable 
	{
	public:
		AT_OrderTable(void);
		virtual ~AT_OrderTable(void);
	};
}


