#pragma  once
#pragma  pack(push)
#pragma  pack(8)
#include "AT_Struct.h"
#include "TradeSignal.h"
namespace AT
{
	struct TradeCommand
	{
	};

	struct InvalidOrderCommand
	{

	};
	struct InvalidCommand :public TradeCommand
	{
		InvalidOrderCommand	m_operation;
	};

	struct InputCommand :public TradeCommand
	{
		InputOrder	m_operation;
	};
	struct CancelCommand :public TradeCommand
	{
		CancelOrder	m_operation;
	};

	struct ModifyCommand :public TradeCommand
	{
		ModifyOrder	m_operation;
	};

}
#pragma  pack(pop)