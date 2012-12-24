#pragma once
#include<map>
#include <boost\shared_ptr.hpp>


#include "ItemTable.h"

#include "CTP_API.h"

namespace CTP
{

	typedef boost::shared_ptr<CThostFtdcProductField> ProductPtr;


	class CThostFtdcProductFieldTraits
	{
	public:
		static std::string GetItemID(ProductPtr apProduct){return std::string(apProduct->ProductID,30);};
		static std::string GetItemTypeName(){return "CThostFtdcProductField";};
	};

	class ProductTable_CTP :public ItemTable<CThostFtdcProductField , CThostFtdcProductFieldTraits>
	{
	public:

		ProductTable_CTP(void);
		virtual ~ProductTable_CTP(void);
		void AddProduct(ProductPtr);
		ProductPtr GetProduct(const std::string& aProductID);

	};
}

