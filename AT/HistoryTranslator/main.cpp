#include <iostream>

#include "MarketDataTranslator.h"

int main(int argc , char** argv)
{
	if(argc <3)
	{
		std::cout<<"�������� ��һ������ΪҪ������Ŀ¼ �ڶ�������Ϊ�����Ŀ¼";
	}
	std::cout<<"TransLator begin\n";
	MarketDataTranslator linst(argv[2]);
	linst.PraseDir(argv[1]);
	return 0;

}