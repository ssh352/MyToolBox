#pragma once

#include <string>
#include <map>

#include "AT_API_HEAD.h"
namespace AT
{
	class ITradeSpi;

	class IDriver_TD
	{
	public:

		virtual void UpdateParam(const std::string& apParam) {};
		virtual void Start(){};
		virtual void Stop(){};

		virtual std::string CreateOrder(const std::string& aNewOrder) =0;
		virtual void DeleteOrder(const std::string& aClientOrderID) =0;
		virtual	void ModifyOrder(const std::string& aRequest) =0;
		virtual void QueryPosition(const std::string& aRequest) =0;
	
	};

}

typedef AT::IDriver_TD* (*CreatTDInstFun)(const std::string & aConfigFile, AT::ITradeSpi* apTradeSpi);
DLL_API AT::IDriver_TD* CreateTD(const std::string & aConfigFile, AT::ITradeSpi* apTradeSpi);

