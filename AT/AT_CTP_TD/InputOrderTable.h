#pragma once

#include <map>
#include <boost/shared_ptr.hpp>
#include "ItemTable.h"
#include "CTP_API.h"
namespace CTP
{
	
	class CThostFtdcInputOrderFieldTraits
	{
	public:
		static std::string GetItemID(InputOrderTypePtr apOrder)
		{
			return GenerateThostOrderID(apOrder,m_FrontID,m_SessionID);
		}
		static std::string GetItemTypeName(){return "CThostFtdcOrderField";};
		static const int IsDuplicateException = 1;

		static void SetFrontID(int aFrontID){m_FrontID = aFrontID;}
		static void SetSessionID(int aSessionID){m_SessionID = aSessionID;}
		static int m_SessionID;
		static int m_FrontID;
	};


	class InputOrderTable_CTP :public ItemTable<CThostFtdcInputOrderField,CThostFtdcInputOrderFieldTraits>
	{
	public:
		void AddOrder(InputOrderTypePtr apOrder );
		InputOrderTypePtr GetOrder(const std::string& aThostOrderID);
	};
}


