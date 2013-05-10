#pragma  once

#include "AT_Struct.h"
#include "TradeSignal.h"
namespace AT
{
	struct TradeCommand
	{
		enum class OperatonType
		{
			NewTradeCommand,
			ModifyTradeCommand,
			CancelTradeCommand
		};

	
		char InstrumentID[cInstrimentIDLength];
		BuySellType m_BuySellType;
		uint32_t m_VolFactor;
		int32_t m_Price;
		TradeSignal m_TradeSignal;
		OperatonType m_OperationType;
	};


}