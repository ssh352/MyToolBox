#include <iostream>

#include "MarketDataTranslator.h"

int main(int argc , char** argv)
{
	if(argc <3)
	{
		std::cout<<"参数不够 第一个参数为要解析的目录 第二个参数为保存的目录";
	}
	std::cout<<"TransLator begin\n";
	MarketDataTranslator linst(argv[2]);
	linst.PraseDir(argv[1]);
	return 0;

}