#include "OrderTable_CTP.h"
#include "CTP_API.h"
namespace CTP
{
	OrderTable_CTP::OrderTable_CTP(void)
	{
	}


	OrderTable_CTP::~OrderTable_CTP(void)
	{
	}

	void OrderTable_CTP::AddOrder( OrderTypePtr aOrderPtr )
	{
		
		std::string lExchangeOrderID = GenerateExchangeOrderID(aOrderPtr);
		std::string lCilentOrderID = GenerateClientOrderID(aOrderPtr);

		{
			boost::mutex::scoped_lock(m_Lock);
			m_OrderMap [lExchangeOrderID] = aOrderPtr;
			m_ClientOrderID_ExchangeID[lCilentOrderID] = lExchangeOrderID;
			m_ExchangeID_ClientOrderID[lExchangeOrderID] = lCilentOrderID;
		}
	}

	void OrderTable_CTP::UpdateOrder( OrderTypePtr aOrderPtr )
	{
		std::string lExchangeOrderID = GenerateExchangeOrderID(aOrderPtr);
		{
			boost::mutex::scoped_lock(m_Lock);
			m_OrderMap [lExchangeOrderID] = aOrderPtr;
		}

	}


	OrderTable_CTP::OrderTypePtr OrderTable_CTP::FindOrderByClientOrderID( const std::string & aClientOrderID )
	{
		return m_OrderMap [m_ClientOrderID_ExchangeID[aClientOrderID]];
	}

	OrderTable_CTP::OrderTypePtr OrderTable_CTP::FindOrderByExchangeID( const std::string & aExchangeOrderID )
	{
		return m_OrderMap [aExchangeOrderID];
	}


	void OrderTable_CTP::TraverseAll( OrderTableTraver aTraver )
	{
		boost::mutex::scoped_lock(m_Lock);
		for(std::map<std::string,OrderTypePtr>::iterator iter = m_OrderMap.begin();iter!= m_OrderMap.end()
			;++iter)
		{
			aTraver(iter->second);
		}
	}
}






