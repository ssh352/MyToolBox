#pragma once


#include "AT_API_HEAD.h"
#include "AT_Struct.h"
namespace AT
{
	class IMarketSpi;

	class IDriver_MD
	{
	public:

		virtual void UpdateParam(const Param& apParam) {};
		virtual  void Start(){};
		virtual	void Stop(){};


	};

}

typedef AT::IDriver_MD* (*CreatMDInstFun)(const char* aConfigFilaName, AT::IMarketSpi* apTradeSpi);

DLL_API AT::IDriver_MD* CreateMD(const char* aConfig, AT::IMarketSpi* apTradeSpi);
