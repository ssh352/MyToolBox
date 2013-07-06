#pragma once
#include "AT_Struct.h"
#include "Signal.h"
namespace AT
{
	class ITradeSignalFliter
	{
	public:
		//�����������裬��һ���Ǻϲ����ڶ����ǹ���
		virtual Signal FliterTradeSignal(std::vector<Signal> aList) = 0;

		//�������µ�ʱ��������
		virtual void OnMarketDepth(const AT::MarketData&  aMarket) = 0;

		//��������������ɶԵ��ã���������棬�ֳ�2����Ϊ�˸���С����
		virtual void UpdateProfit(int32_t aProfit,AT_Time aTime) = 0;//֪ͨһ���źŵ�ӯ��
		virtual void OnAccountCompleteSignal() = 0; //֪ͨһ���źŽ���
	};
}