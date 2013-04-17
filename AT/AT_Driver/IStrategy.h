#pragma once
#include <string>

#include "AT_API_HEAD.h"

namespace AT
{
	class IStrategySpi;
	class IDriver_TD;
	class IStrategy 
	{
	public:

		enum EStrInputState
		{
			PARAM,
			MDSTATE,
			TDSTATE
		};
		virtual void UpdateParam(EStrInputState errCode ,const std::string& apParam) {};
		virtual void Start() {};
		virtual void Stop(){};
	public:
		virtual void OnMarketDepth(const std::string& aMarketDepth)= 0;
		virtual void OnRtnOrder(const std::string& apOrder) = 0;
		virtual void OnRtnTrade(const std::string& apTrade)=0;
		virtual void OnRtnPos(const std::string& aPos) = 0;


	};
}
typedef  AT::IStrategy* (*CreateStrInstFun) (const std::string aConfigFile, AT::IDriver_TD* apTradeSpi, AT::IStrategySpi* aStrSpi);
DLL_API  AT::IStrategy* CreateStr(const std::string aConfigFile, AT::IDriver_TD* apTradeSpi ,AT::IStrategySpi* aStrSpi);