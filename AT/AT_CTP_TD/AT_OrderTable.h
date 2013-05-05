#pragma once
#include "ItemTable.h"
#include "AT_Struct.h"

namespace AT
{
	class OrderUpdateTraits
	{
	public:
		typedef AT_Order_Key KeyType ;

		static KeyType GetItemKey(std::shared_ptr<AT::OrderUpdate> apOrder)
		{
			return apOrder->m_Key;
		}
	};

	
	class AT_OrderTable :public  ItemTable<AT::OrderUpdate,OrderUpdateTraits>
	{
	public:
		AT_OrderTable(const char* aOrderTableDBpaht);
		virtual ~AT_OrderTable();
		std::shared_ptr<AT::OrderUpdate> FindOrderByPlatformOrderID(const std::string& aPlatfromORderID);
		std::shared_ptr<AT::OrderUpdate> FindOrderByExchangeOrderID(const std::string& aExchangORderID);

	};
	
}


