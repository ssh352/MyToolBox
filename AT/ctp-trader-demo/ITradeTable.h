#pragma once
#include <boost/shared_ptr.hpp>
#include <boost/function.hpp>




template<typename TradeType>
class ITradeTable
{
public:
	typedef boost::shared_ptr<TradeType> TradeTypePtr;
	typedef boost::function< void (TradeTypePtr) > TradeTableTraver;

public:
	virtual void AddTrade( TradeTypePtr aOrderPtr) = 0;
	//find by Product
	//find by user
	//find by Instrument
	virtual void TraverseAll(TradeTableTraver aTraver) = 0;

};