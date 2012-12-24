#include "ProductTable_CTP.h"
#include "CTP_API.h"
namespace CTP
{
	void ProductTable_CTP::AddProduct( ProductPtr  apProduct)
	{
		AddItem(apProduct);
	}

	ProductPtr ProductTable_CTP::GetProduct( const std::string& aProductID )
	{
		return GetItem(aProductID);
	}
}

