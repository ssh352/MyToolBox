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

		//初始设置单边持仓限额
		virtual void InitSignalDirectionVol(){m_BuyDirectionVol = 0;m_SellDirectionVol = 0;}
		//设置单边持仓限额
		virtual void SetSignalDirectionVol(BuySellType type,int Vol)
		{
			if(type == BuySellType::BuyOrder)
				m_BuyDirectionVol += Vol;
			else
				m_SellDirectionVol += Vol;
		}
		//获取单边持仓限额
		virtual int GetSignalDirectionVol(BuySellType type)
		{
			if(type == BuySellType::BuyOrder)
			{
				return m_BuyDirectionVol;
			}
			return m_SellDirectionVol;
		}
		//初始化总的交易次数
		virtual void InitTotalTime(){m_TotalTime = 0;}
		//设置交易次数
		virtual void SetTotalTime(){m_TotalTime++;}
		//获取交易次数
		virtual int  GetTotalTime(){return m_TotalTime;}

		//初始化总的撤单交易次数
		virtual void InitTotalCancleTime(){m_TotalCancleTime = 0;}
		//设置撤单交易次数
		virtual void SetTotalCancleTime(){m_TotalCancleTime++;}
		//获取撤单交易次数
		virtual int GetTotalCancleTime(){return m_TotalCancleTime;}
	protected:
		FinishExecuteCallbackType  m_FinishehNotfiy;
		int						   m_SellDirectionVol;
		int						   m_BuyDirectionVol;
		int						   m_TotalTime;
		int						   m_TotalCancleTime;
	};


}