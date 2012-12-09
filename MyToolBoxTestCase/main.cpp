#include "googleMock/gmock/gtest/gtest.h"
#include "googleMock/gmock/gmock.h"

#include <iostream>
#include "PriceNumber.h"
using namespace MY_TOOL_BOX;

TEST(INST_PriceNumber,xxx)
{
	PriceNumber inst(123);
}

int main(int argc,char** argv)
{
	testing::InitGoogleMock(&argc,argv);
	return RUN_ALL_TESTS();
}