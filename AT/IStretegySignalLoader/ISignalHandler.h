#pragma  once
namespace AT
{


	class ITradeSignalDispacher;
	//���ܸ�����ָͬ����ź� Ȼ����ϳ�Ϊ���ײ��Ե�ָ��
	class ISignalHandler
	{
		virtual void RegiestTradeCommandDispacher(ITradeSignalDispacher* apDispacher) = 0;
	};
}
