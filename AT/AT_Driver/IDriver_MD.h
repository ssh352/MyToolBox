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

		virtual void UpdateParam(const std::string& apParam) {};
		virtual  void Start(){};
		virtual	void Stop(){};


	};

}

typedef AT::IDriver_MD* (*CreatMDInstFun)(const std::string& aConfig, AT::IMarketSpi* apTradeSpi);

DLL_API AT::IDriver_MD* CreateMD(const std::string& aConfig, AT::IMarketSpi* apTradeSpi);
