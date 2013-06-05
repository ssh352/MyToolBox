#pragma  once
#include "AT_Struct.h"
#include "TradeSignal.h"
namespace AT
{
	class ITradeSignalExecutor 
	{
	
	public:

		//����1 �������ϲ�Ľ����ź�
		virtual void HandleTradeSignal(const TradeSignal& aSignal) =0;

		//����2 �����ź�
		virtual void OnMarketDepth(const AT::MarketData& aMarketDepth) =0;

		//����3 ����������ģ���֪ͨ
		virtual void NotifyTraded(const TradeSignal& aSignal , int32_t accountIndex) =0;
		virtual void NotifyCancel(const TradeSignal& aSignal , int32_t accountIndex) = 0;
		//void NotifyTrade(const TradeSignal& aSignal , int32_t accountIndex);

	};


}