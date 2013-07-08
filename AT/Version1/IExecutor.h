#pragma  once
#include "AT_Struct.h"
#include "Command.h"
#include <boost\function.hpp>
namespace AT
{

	struct ExecutorInput
	{
		BuySellType IsBuy;
		OpenCloseType IsOpen;
		int vol;
		char InstrumentID[cInstrimentIDLength];
		AT::MarketData LastMarketData;
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
		int AddTastVol; //�ܵ�����������ƽ��ʱ�����ۼӶ��ƽ�������������
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
			IsFinised = false;
		}
	};


	typedef boost::function<void(ExecutionResult)> TradeReportFun;
	class IExecutor 
	{

	public:
		//����1 ����������
		virtual Command AddExecution(ExecutorInput aExecutorInput) = 0; //���������
		virtual Command Abrot() = 0;									//��ִֹ�� ������
		//����2 ������ִ�в���
		virtual Command OnMarketDepth(const AT::MarketData& aMarketDepth) =0;
		virtual Command  OnRtnOrder(const  AT::OrderUpdate& apOrder) =0;
		virtual Command OnRtnTrade(const  AT::TradeUpdate& apTrade) =0;
		//��ѯִ��״̬�Ƿ����
		virtual ExecutionStatus	GetExecutionStatus() = 0;

		//����3 ������������callback
		virtual void SetTradeReportCallback (TradeReportFun aFinishCallback) final {m_TradeReport = aFinishCallback;} ;
		virtual std::string GetExecutorID() = 0;


	protected:
		TradeReportFun				m_TradeReport;
		std::string					m_ExecutorID;
	};


}