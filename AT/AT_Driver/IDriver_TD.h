#pragma once

#include <string>
#include <map>

namespace AT
{
	class ITradeSpi;

	class IDriver_TD
	{

	public:
		virtual void Init(const std::map<std::string,std::string>& aConfigMap, AT::ITradeSpi* apTradeSpi) =0;
	public:
		virtual std::string CreateOrder(const std::string& aNewOrder) =0;
		virtual void DeleteOrder(const std::string& aClientOrderID) =0;
		virtual	void ModifyOrder(const std::string& aRequest) =0;
		virtual void QueryPosition(const std::string& aRequest) =0;
	
	};

}

typedef AT::IDriver_TD* (*CreatInstFun)(const std::map<std::string,std::string>& aConfig, AT::ITradeSpi* apTradeSpi);
#ifdef IDRIVER_LIB
extern "C" __declspec(dllexport) AT::IDriver_TD* CreateDriverInsance(const std::map<std::string,std::string>& aConfig, AT::ITradeSpi* apTradeSpi);
#else
extern "C" __declspec(dllimport) AT::IDriver_TD* CreateDriverInsance(const std::map<std::string,std::string>& aConfig, AT::ITradeSpi* apTradeSpi);
#endif
