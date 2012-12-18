#include "stdafx.h"

#include <boost\format.hpp>

TEST(boost_format,simpale)
{
	int val = 1234;
	boost::format fmt("%1%");
	fmt % boost::io::group(std::setfill('0'),std::setw(12),val);
	std::string lRet = str(fmt);
	EXPECT_EQ(std::string("000000001234"),lRet);
}