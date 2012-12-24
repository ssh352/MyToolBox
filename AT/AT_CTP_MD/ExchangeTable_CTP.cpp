#include "ExchangeTable_CTP.h"
#include "CTP_API.h"

namespace CTP
{
	void ExchangeTable_CTP::AddExchange( Exchange_Ptr apExchange )
	{
		AddItem(apExchange);
	}

	Exchange_Ptr ExchangeTable_CTP::GetExchange( const std::string& aExchangeID )
	{
		return GetItem(aExchangeID);
	}

}

