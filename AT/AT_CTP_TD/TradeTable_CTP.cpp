#include "TradeTable_CTP.h"

namespace CTP
{
	void TradeTable_CTP::AddTrade( Trade_Ptr apOrder )
	{
		AddItem(apOrder);
	}

	CTP::Trade_Ptr TradeTable_CTP::GetTrade( const std::string& aTradeID )
	{
		return  GetItem(aTradeID);
	}

}
