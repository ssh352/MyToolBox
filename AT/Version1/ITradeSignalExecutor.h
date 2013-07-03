#pragma  once
#include "AT_Struct.h"
#include "TradeCommand.h"
#include <boost\smart_ptr.hpp>
#include <boost\function.hpp>
namespace AT
{


	typedef boost::function<void(int32_t Price,int32_t vol,bool IsBuy, bool isFinish)> FinishExecuteCallbackType;
	class ITradeSignalExecutor 
	{
	
	public:

		//输入1 来自于上层的交易信号
		virtual boost::shared_ptr<TradeCommand> SetupTarget(int targetQuantity,bool IsBuy, const AT::MarketData& aMarket) =0;
		virtual boost::shared_ptr<TradeCommand> AddTarget(int addTargetQuantity, bool isBuy, const AT::MarketData& aMarket) =0;

		//输入2 来自于执行层面
		virtual  boost::shared_ptr<TradeCommand> OnMarketDepth(const AT::MarketData& aMarketDepth) =0;
		virtual  boost::shared_ptr<TradeCommand> OnRtnOrder(const  AT::OrderUpdate& apOrder) =0;
		virtual  boost::shared_ptr<TradeCommand> OnRtnTrade(const  AT::TradeUpdate& apTrade) =0;

		//输入3 设置完成任务的callback
		//
		virtual void SetFinishedCallback(FinishExecuteCallbackType aFinishCallback) {m_FinishehNotfiy = aFinishCallback;};
		virtual std::string GetExecutorID() = 0;

		
	protected:
		FinishExecuteCallbackType  m_FinishehNotfiy;
	};


}