#include "TradeTable_CTP.h"
#include "CTP_API.h"
namespace CTP
{

	void TradeTable_CTP::AddTrade( TradeTypePtr aTradePtr )
	{
		std::string lTradeID = GenerateTradeID(aTradePtr);
		{
			boost::mutex::scoped_lock(m_Lock);
			m_TradeMap[lTradeID] = aTradePtr;
		}

	}

	void TradeTable_CTP::TraverseAll( TradeTableTraver aTraver )
	{
		boost::mutex::scoped_lock(m_Lock);
		for(std::map<std::string,TradeTypePtr>::iterator iter = m_TradeMap.begin();iter!= m_TradeMap.end()
			;++iter)
		{
			aTraver(iter->second);
		}
	}

}
