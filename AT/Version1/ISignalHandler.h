#pragma  once
namespace AT
{


	class ITradeSignalDispacher;
	//接受各个不同指标的信号 然后将其合成为交易策略的指令
	class ISignalHandler
	{
		virtual void RegiestTradeCommandDispacher(ITradeSignalDispacher* apDispacher) = 0;
	};
}
