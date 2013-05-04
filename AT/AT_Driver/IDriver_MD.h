#pragma once


#include "AT_API_HEAD.h"
#include "AT_Struct.h"
namespace AT
{
	class IMarketSpi;

	class IDriver_MD
	{
	public:
		virtual void UpdateParam(const AT::Param& apParam) = 0;
		virtual  void Start() = 0;
		virtual	void Stop() = 0;
	};

}

typedef AT::IDriver_MD* (*CreatMDInstFun)(const char* aConfigFilaName, AT::IMarketSpi* apTradeSpi);

DLL_API AT::IDriver_MD* CreateMD(const char* aConfig, AT::IMarketSpi* apTradeSpi);
