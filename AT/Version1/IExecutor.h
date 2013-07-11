#pragma  once
#include "AT_Struct.h"
#include "Command.h"
#include <boost\function.hpp>
namespace AT
{

	struct ExecutorInput
	{
		BuySellType BuySellCode;
		OpenCloseType OpenCloseCode;
		int vol;
		char InstrumentID[cInstrimentIDLength];
		AT::MarketData TriggerMarketData;
	};

	struct ExecutionResult
	{
		BuySellType IsBuy;
		OpenCloseType IsOpen;
		int32_t vol;
		char InstrumentID[cInstrimentIDLength];
		int32_t Price;
	};

	struct ExecutionStatus
	{
		int AddTastVol; //总的任务数量
		int TradeVol;	//已经成交的数量
		int LivelVol;	//还激活在交易所的数量
		int SuspendVol_Exechange; //在交易所挂起的数量 如挂到交易所得部分单子
		int	SuspendVol_Local;	 //本地挂起的数量
		int CancelVol;
		bool IsFinised; //是否已经完成（放弃也属于完成）
		ExecutionStatus()
		{
			AddTastVol = 0;
			TradeVol = 0;	
			LivelVol = 0;
			SuspendVol_Exechange = 0;
			SuspendVol_Local = 0;	
			IsFinised = true;
		}
	};


	typedef boost::function<void(ExecutionResult)> TradeReportFun;
	typedef boost::function<void(Command)>			CommandHandler;
	class IExecutor 
	{

	public:
		//输入1 来自于命令
		virtual	void AddExecution(ExecutorInput aExecutorInput) = 0; //添加新任务
		virtual void Abrot() = 0;									//终止执行 并撤单
		//输入2 来自于执行层面
		virtual	void OnMarketDepth(const AT::MarketData& aMarketDepth) =0;
		virtual void OnRtnOrder(const  AT::OrderUpdate& apOrder) =0;
		virtual void OnRtnTrade(const  AT::TradeUpdate& apTrade) =0;
		//查询执行状态是否结束
		virtual ExecutionStatus	GetExecutionStatus() = 0;

		//输入3 设置完成任务的callback
		void SetTradeReportCallback (TradeReportFun aFinishCallback)  {m_TradeReport = aFinishCallback;} ;
		virtual std::string GetExecutorID() = 0;

		//输入4 设置一个接受Command的回掉，因为有可能一个更新要发出多个Command
		//比如开仓成功的同时要下一个平仓单，同时开仓部分自己也可能会有单子。
		void SetCommandHandler(CommandHandler aCommandHandler)
		{
			m_CommandHandle = aCommandHandler;
		}
		


	protected:
		CommandHandler				m_CommandHandle;
		TradeReportFun				m_TradeReport;
		std::string					m_ExecutorID;
	};


}