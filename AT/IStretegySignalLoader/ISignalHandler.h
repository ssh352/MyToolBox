#pragma  once
namespace AT
{


	class ITradeSignalDispacher;
	//���ܸ�����ָͬ����ź� Ȼ����ϳ�Ϊ���ײ��Ե�ָ��
	class ISignalHandler
	{
		//todo 
		//should consider how to receive signal

		virtual void RegiestTradeCommandDispacher(ITradeSignalDispacher* apDispacher) = 0;
	};
}
