#include "OrderTable_CTP.h"

namespace CTP
{


	void OrderTable_CTP::AddOrder( Order_Ptr apOrder )
	{
		AddItem(apOrder);
	}

	CTP::Order_Ptr OrderTable_CTP::GetOrder( const std::string& aThostOrderID )
	{
		return GetItem(aThostOrderID);
	}

}