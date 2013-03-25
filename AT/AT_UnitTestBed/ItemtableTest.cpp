#include "stdafx.h"
#include "CTP_TD.h"
#include "DataCache_CTP_TD.h"
#include <iostream>

std::string GetCmd(std::istream* lInput)
{
	char lCmd[512] ;
	memset(lCmd,0,512);
	lInput->getline(lCmd,500,'\n');
	std::string lRet(lCmd);
	return lRet;
}

TEST(ItemTablecase,test1)
{
	CTP::DataCache_CTP_TD ldatacache("Test");
	boost::shared_ptr<CThostFtdcOrderField> lOrderField(new CThostFtdcOrderField);

	memset(lOrderField.get(),0,sizeof(CThostFtdcOrderField));
	lOrderField->SessionID = 12345;
	lOrderField->FrontID = 10;
	strcpy(lOrderField->OrderRef,"123");

	ldatacache.UpdataOrder(lOrderField);

	std::stringstream linputBUf;
	linputBUf<<"2 12345_10_123\n";
	int mode ;
	linputBUf>>mode;
	std::string lClientOrderID = GetCmd(&linputBUf);
	boost::shared_ptr<CThostFtdcOrderField> lRet = ldatacache.GetOrderByThostID(lClientOrderID);
	EXPECT_EQ(lRet,lOrderField);

}