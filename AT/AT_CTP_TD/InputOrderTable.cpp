#include "InputOrderTable.h"
namespace CTP
{

	int CThostFtdcInputOrderFieldTraits::m_FrontID = 0;
	int CThostFtdcInputOrderFieldTraits::m_SessionID = 0;
	void InputOrderTable_CTP::AddOrder( InputOrderTypePtr apOrder )
	{
		AddItem(apOrder);
	}

	InputOrderTypePtr  InputOrderTable_CTP::GetOrder( const std::string& aThostOrderID )
	{
		return GetItem(aThostOrderID);
	}

}