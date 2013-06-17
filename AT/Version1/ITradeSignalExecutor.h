#pragma  once
#include "AT_Struct.h"
#include "TradeCommand.h"
#include <boost\smart_ptr.hpp>
namespace AT
{
	class ITradeSignalExecutor 
	{
	
	public:

		//输入1 来自于上层的交易信号
		virtual boost::shared_ptr<TradeCommand> SetupTarget(int targetQuantity,const AT::MarketData& aMarket) =0;
		virtual boost::shared_ptr<TradeCommand> AddTarget(int addTargetQuantity, const AT::MarketData& aMarket) =0;

		//输入2 来自于执行层面
		virtual  boost::shared_ptr<TradeCommand> OnMarketDepth(const AT::MarketData& aMarketDepth) =0;
		virtual  boost::shared_ptr<TradeCommand> OnRtnOrder(const  AT::OrderUpdate& apOrder) =0;
		virtual  boost::shared_ptr<TradeCommand> OnRtnTrade(const  AT::TradeUpdate& apTrade) =0;

		//
		virtual std::string GetExecutorID() = 0;
	};


}