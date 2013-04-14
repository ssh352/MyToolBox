#pragma once

#include <string>
#include <map>

#include "AT_API_HEAD.h"
namespace AT
{
	class IMarketSpi;

	class IDriver_MD
	{
	public:
		virtual  void Start(){};
		virtual	void Stop(){};


	};

}

typedef AT::IDriver_MD* (*CreatMDInstFun)(const std::map<std::string,std::string>& aConfig, AT::IMarketSpi* apTradeSpi);

DLL_API AT::IDriver_MD* CreateMD(const std::string& aConfig, AT::IMarketSpi* apTradeSpi);
