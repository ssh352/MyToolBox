#include "ExchangeTable_CTP.h"
#include "CTP_API.h"

namespace CTP
{

	ExchangeTable_CTP::ExchangeTable_CTP(void)
	{
	}


	ExchangeTable_CTP::~ExchangeTable_CTP(void)
	{
	}

	void ExchangeTable_CTP::AddExchange( Exchange_Ptr apExchange )
	{
		std::string lExchangeName (apExchange->ExchangeID,8);
		if(m_ExchangeMap.count(lExchangeName))
		{
			std::string Exception_info_str = "Add Exchange Already Exist ExchangeId=";
			Exception_info_str += lExchangeName;
			throw std::exception(lExchangeName.c_str());
		}
		else
		{
			m_ExchangeMap.insert(make_pair(lExchangeName,apExchange));
		}
		
	}

	Exchange_Ptr ExchangeTable_CTP::GetExchange( const std::string& aExchangeID )
	{
		if(m_ExchangeMap.count(aExchangeID))
		{
			return m_ExchangeMap[aExchangeID];
		}
		else
		{
			return Exchange_Ptr();
		}
	}


}

