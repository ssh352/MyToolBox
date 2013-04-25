#pragma once

#include "AT_API_HEAD.h"

#include "AT_Struct.h"
namespace AT
{
	class ITradeSpi;

	class IDriver_TD
	{
	public:

		virtual void UpdateParam(const Param& apParam) {};
		virtual void Start(){};
		virtual void Stop(){};

		virtual void CreateOrder(const NewOrder& aNewOrder) =0;
		virtual void DeleteOrder(const CancelOrder& aDelOrderID) =0;
		virtual	void ModifyOrder(const ModifyOrder& aRequest) =0;
		//virtual void QueryPosition(const std::string& aRequest) =0;
	
	};

}

typedef AT::IDriver_TD* (*CreatTDInstFun)(const char*  aConfigFile, AT::ITradeSpi* apTradeSpi);
DLL_API AT::IDriver_TD* CreateTD(const char* aConfigFile, AT::ITradeSpi* apTradeSpi);

