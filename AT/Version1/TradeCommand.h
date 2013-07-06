#pragma  once
#pragma  pack(push)
#pragma  pack(8)
#include "AT_Struct.h"
#include "Signal.h"
namespace AT
{

	enum class TradeCommandType
	{
		Invalid,
		Input,
		Modify,
		Cancel
	};

	struct TradeCommand
	{
	

		TradeCommand(TradeCommandType aType)
			:m_CommandType(aType)
		{

		}
		TradeCommandType m_CommandType;
	};

	struct InvalidOrderCommand
	{

	};
	struct InvalidCommand :public TradeCommand
	{
		InvalidCommand()
			: TradeCommand(TradeCommandType::Invalid)
		{

		}
		InvalidOrderCommand	m_operation;
	};

	struct InputCommand :public TradeCommand
	{
		InputCommand()
			: TradeCommand(TradeCommandType::Input)
		{

		}
		InputOrder	m_operation;
	};
	struct CancelCommand :public TradeCommand
	{
		CancelCommand()
			: TradeCommand(TradeCommandType::Cancel)
		{

		}
		CancelOrder	m_operation;
	};

	struct ModifyCommand :public TradeCommand
	{
		ModifyCommand()
			: TradeCommand(TradeCommandType::Modify)
		{

		}
		ModifyOrder	m_operation;
	};

}
#pragma  pack(pop)