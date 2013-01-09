#include "PositionTable_CTP.h"

namespace CTP
{


	void PositionTable_CTP::AddPostion( Postion_Ptr apPosition )
	{
		AddItem(apPosition);
	}

	Postion_Ptr PositionTable_CTP::GetPosition( const std::string& aPositionID )
	{
		return GetItem(aPositionID);
	}

	void PositionTable_CTP::ClearPosition()
	{
		Clear();
	}

	void PositionTable_CTP::DelPosition( const std::string& aPositionID )
	{
		DelItem(aPositionID);
	}

	void PositionTable_CTP::DelPosition( Postion_Ptr apPosition )
	{
		std::string lPosID = CThostFtdcInvestorPositionFieldTraits::GetItemID(apPosition);
		DelPosition(lPosID);
	}

}
