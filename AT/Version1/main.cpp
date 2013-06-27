

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>
#include <boost/format.hpp>
#include "../AT_CTP_MD/CTP_MD.h"
#include "../AT_CTP_Multi_TD/CTP_MULTI_TD.h"
//#include "../AT_CTP_TD/CTP_TD.h"
#include "version1Container.h"
#include "main_versionLoaderPoster.h"
#include <boost\thread.hpp>

#include <atomic>
std::atomic<bool> g_IsMarketFinish = false;

int main()
{
	AT::main_versionLoaderPoster versionPoster(g_IsMarketFinish);

//	AT::IDriver_TD* pTD = new CTP::CTP_TD("../AT_CTP_TD/TDConfig.xml",&versionPoster);
	AT::IDriver_TD* pTD = new CTP::CTP_MULTI_TD("../AT_CTP_Multi_TD/Multi_TD.xml",&versionPoster);
	AT::IDriver_MD* pMD = new CTP::CTP_MD("../AT_CTP_MD/MDConfig.xml",&versionPoster);

	AT::version1Container* pVersionContainer = new AT::version1Container("",pTD,&versionPoster,pMD->GetMarketCache());

	versionPoster.Setup(pVersionContainer);

	pTD->Start();
	pVersionContainer->Start();
	pMD->Start();

	while(!g_IsMarketFinish)
	{
		boost::this_thread::sleep(boost::posix_time::seconds(1));
	}
	pTD->Stop();
	pVersionContainer->Stop();
	pMD->Stop();

	system("pause");
	return 0;
}