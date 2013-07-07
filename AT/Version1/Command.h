#pragma  once
#pragma  pack(push)
#pragma  pack(8)
#include "AT_Struct.h"
#include "Signal.h"
namespace AT
{
	enum class CommandType
	{
		Invalid,
		Input,
		Modify,
		Cancel
	};

	struct Command
	{
		CommandType		m_CommandType;
		InputOrder	m_InputOrder;
		CancelOrder	m_CancelOrder;
		ModifyOrder	m_ModifyOrder;
	};

	static Command InvalidCommand = {CommandType::Invalid};


}
#pragma  pack(pop)