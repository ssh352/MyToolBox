#pragma  once
namespace AT
{


	class ITradeSignalDispacher;
	//接受各个不同指标的信号 然后将其合成为交易策略的指令
	class ISignalHandler
	{
		//todo 
		//should consider how to receive signal

		virtual void RegiestTradeCommandDispacher(ITradeSignalDispacher* apDispacher) = 0;
	};
}
