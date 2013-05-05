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
	typedef  ItemTable<AT::OrderUpdate,OrderUpdateTraits>  AT_OrderTable ;
}


