#include "PositionTable_CTP.h"

namespace CTP
{


	void PositionTable_CTP::AddPostion( Postion_Ptr apPosition )
	{
		AddItem(apPosition);
	}

	Postion_Ptr PositionTable_CTP::GetPostion( const std::string& aPositionID )
	{
		return GetItem(aPositionID);
	}

	void PositionTable_CTP::ClearPosition()
	{
		Clear();
	}

}
