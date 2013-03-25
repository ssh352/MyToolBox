#include "stdafx.h"

#include "DataCache_CTP_TD.h"
#include <string>

TEST(order_table,store_get)
{

	CTP::DataCache_CTP_TD ltable("TestTable");
	boost::shared_ptr<CThostFtdcOrderField>  apOrder(new CThostFtdcOrderField());

	apOrder->SessionID  = 1234123;
	apOrder->FrontID = 2;
	strcpy(apOrder->OrderRef ,"1");
	const std::string lClientOrderID = CTP::GenerateThostOrderID(apOrder);
	ltable.UpdataOrder(apOrder);
	boost::shared_ptr<CThostFtdcOrderField> GetPtr = ltable.FindOrderByThostID(lClientOrderID);
	EXPECT_EQ(GetPtr,apOrder);
}