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

		//����1 �������ϲ�Ľ����ź�
		virtual boost::shared_ptr<TradeCommand> SetupTarget(int targetQuantity,bool IsBuy, const AT::MarketData& aMarket) =0;
		virtual boost::shared_ptr<TradeCommand> AddTarget(int addTargetQuantity, bool isBuy, const AT::MarketData& aMarket) =0;

		//����2 ������ִ�в���
		virtual  boost::shared_ptr<TradeCommand> OnMarketDepth(const AT::MarketData& aMarketDepth) =0;
		virtual  boost::shared_ptr<TradeCommand> OnRtnOrder(const  AT::OrderUpdate& apOrder) =0;
		virtual  boost::shared_ptr<TradeCommand> OnRtnTrade(const  AT::TradeUpdate& apTrade) =0;

		//����3 ������������callback
		//
		virtual void SetFinishedCallback(FinishExecuteCallbackType aFinishCallback) {m_FinishehNotfiy = aFinishCallback;};
		virtual std::string GetExecutorID() = 0;

		
	protected:
		FinishExecuteCallbackType  m_FinishehNotfiy;
	};


}