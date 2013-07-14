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
		int AddTastVol; //�ܵ���������
		int TradeVol;	//�Ѿ��ɽ�������
		int LivelVol;	//�������ڽ�����������
		int SuspendVol_Exechange; //�ڽ�������������� ��ҵ��������ò��ֵ���
		int	SuspendVol_Local;	 //���ع��������
		int CancelVol;
		bool IsFinised; //�Ƿ��Ѿ���ɣ�����Ҳ������ɣ�
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
		//����1 ����������
		virtual	void AddExecution(ExecutorInput aExecutorInput) = 0; //���������
		virtual void Abrot() = 0;									//��ִֹ�� ������
		//����2 ������ִ�в���
		virtual	void OnMarketDepth(const AT::MarketData& aMarketDepth) =0;
		virtual void OnRtnOrder(const  AT::OrderUpdate& apOrder) =0;
		virtual void OnRtnTrade(const  AT::TradeUpdate& apTrade) =0;
		//��ѯִ��״̬�Ƿ����
		virtual ExecutionStatus	GetExecutionStatus() = 0;

		
		virtual void SetTradeReportCallback (TradeReportFun aFinishCallback)  = 0; // ���óɽ��ر�
		virtual  void SetCommandHandler(CommandHandler aCommandHandler) = 0;	//����ָ���ӿ�

		virtual std::string GetExecutorID() = 0;

		
	};


}