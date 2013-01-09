#pragma once

#include <map>
#include <boost/shared_ptr.hpp>
#include "ItemTable.h"
#include "CTP_API.h"
namespace CTP
{
	typedef boost::shared_ptr<CThostFtdcInvestorPositionField> Postion_Ptr;

	class CThostFtdcInvestorPositionFieldTraits
	{
	public:
		static std::string GetItemID(Postion_Ptr apPosition)
		{
			std::string lRet;
			lRet += apPosition->InstrumentID;
			lRet += "_";
			lRet += apPosition->PosiDirection;
			return lRet;
		}
		static std::string GetItemTypeName(){return "CThostFtdcInvestorPositionField";};
		static const int IsDuplicateException = 1;
	};


	class PositionTable_CTP :public ItemTable<CThostFtdcInvestorPositionField,CThostFtdcInvestorPositionFieldTraits>
	{
	public:
		void AddPostion(Postion_Ptr apPosition );
		Postion_Ptr GetPostion(const std::string& aPositionID);
		void ClearPosition();
	};
}


