#pragma  once


#include "IOrderTable.h"
#include "ITradeTable.h"

#include <boost/function.hpp>


class IClientOrderIDGenerator;
template<typename InsertOrderType,typename OrderType,typename TradeType>
class ITradeDriver
{
public:
	typedef boost::shared_ptr<OrderType> OrderTypePtr;
	typedef boost::function< void (OrderTypePtr) > OrderUpdater;

	typedef boost::shared_ptr<TradeType> TradeTypePtr;
	typedef boost::function< void (TradeTypePtr) > TradeUpdater;

public:
	virtual void CreateOrder(boost::shared_ptr<InsertOrderType>  aNewOrderPtr) =0;
	virtual void DeleteOrder(std::string ClientOrderId) =0;
	virtual IClientOrderIDGenerator* GetClientOrderIDGeneratorPtr() =0;

	void SetOrderUpdater( OrderUpdater aOrderUpdater){m_OrderUpdater = aOrderUpdater;}
	void SetTraderUpdater(TradeUpdater aTradeUpdater){m_TradeUpdater = aTradeUpdater;}
private:
	TradeUpdater m_TradeUpdater;
	OrderUpdater m_OrderUpdater;
};