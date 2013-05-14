#pragma  once
#include "AT_Struct.h"
#include "TradeSignal.h"
namespace AT
{
	class ITradeSignalExecutor 
	{
		//����1 �������ϲ�Ľ����ź�
		void HandleTradeSignal(const TradeSignal& aSignal);

		//����2 �����ź�
		void OnMarketDepth(const AT::MarketData& aMarketDepth);

		//����3 ����������ģ���֪ͨ
		void NotifyTraded(const TradeSignal& aSignal , int32_t accountIndex);
		void NotifyCancel(const TradeSignal& aSignal , int32_t accountIndex);
		//void NotifyTrade(const TradeSignal& aSignal , int32_t accountIndex);

	};


}