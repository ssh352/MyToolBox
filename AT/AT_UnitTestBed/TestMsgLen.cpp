#include "stdafx.h"
#include <iostream>
#include "CTP_API.h"
TEST(testfile,lens)
{
	std::size_t lsize = sizeof(CThostFtdcDepthMarketDataField);
	std::cout<<lsize;
}