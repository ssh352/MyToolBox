#pragma once

#include "IOrderTable.h"
#include <map>
#include <boost\thread.hpp>



struct CThostFtdcOrderField;

namespace CTP
{

	//TODO for perfomace Should Change To R/W Lock  
	
	class OrderTable_CTP :public IOrderTable<CThostFtdcOrderField> 
	{

	public:
		OrderTable_CTP(void);
		virtual ~OrderTable_CTP(void);

		virtual void AddOrder( OrderTypePtr aOrderPtr) ;
		virtual void UpdateOrder(OrderTypePtr aOrderPtr);
		virtual OrderTypePtr FindOrderByExchangeID(const std::string & aExchangeOrderID) ;
		virtual OrderTypePtr FindOrderByClientOrderID(const std::string & aClientOrderID);
		virtual void TraverseAll(OrderTableTraver aTraver) ;
		virtual boost::mutex& GetMutex()
		{
			return m_Lock;
		};

	private:
		std::map<std::string,OrderTypePtr> m_OrderMap;
		std::map<std::string,std::string> m_ClientOrderID_ExchangeID;
		std::map<std::string,std::string> m_ExchangeID_ClientOrderID;

	private:
		boost::mutex m_Lock;
	};
}




