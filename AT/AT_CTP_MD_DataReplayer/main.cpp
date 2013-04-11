#include "CTP_MD_Replayer.h"
#include "../MD_PrintTest/PrintMDSpi.h"
int main()
{
	CTP::CTP_MD_Replayer inst;

	std::map<std::string,std::string> lConfig;
	lConfig["IF1304"] = "C:\\GitTrunk\\MyToolBox\\AT\\CTP_MD_DataRecoder\\MarketDB20130412IF1304";
	lConfig["IF1305"] = "C:\\GitTrunk\\MyToolBox\\AT\\CTP_MD_DataRecoder\\MarketDB20130412IF1305";
	PrintMDSpi lPrinter;
	inst.Init(lConfig,&lPrinter);
	system("pause");
		
}