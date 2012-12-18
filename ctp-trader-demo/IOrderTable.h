#pragma once
#include <boost/shared_ptr.hpp>
#include <boost/function.hpp>
#include <boost/thread.hpp>



template<typename OrderType>
class IOrderTable
{
public:
	typedef boost::shared_ptr<OrderType> OrderTypePtr;
	typedef boost::function< void (OrderTypePtr) > OrderTableTraver;

public:
	virtual void AddOrder( OrderTypePtr aOrderPtr) = 0;
	virtual void UpdateOrder(OrderTypePtr aOrderPtr) = 0;
	virtual OrderTypePtr FindOrderByExchangeID(const std::string & aExchangeOrderID) = 0;
	virtual OrderTypePtr FindOrderByClientOrderID(const std::string & aClientOrderID) = 0;
	virtual void TraverseAll(OrderTableTraver aTraver) = 0;
	virtual boost::mutex& GetMutex() = 0;
};

