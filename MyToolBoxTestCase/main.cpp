#include "stdafx.h"



int main(int argc,char** argv)
{
	testing::InitGoogleMock(&argc,argv);
	
	 int ret = RUN_ALL_TESTS();
	 system("pause");
	 return ret;


}