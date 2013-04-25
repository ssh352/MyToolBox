#pragma once
#include "AT_API_HEAD.h"
#include "AT_Struct.h"
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
		virtual void UpdateParam(EStrInputState errCode ,const Param& apParam) {};
		virtual void Start() {};
		virtual void Stop(){};
	public:
		virtual void OnMarketDepth(const MarketData& aMarketDepth)= 0;
		virtual void OnRtnOrder(const  OrderUpdate& apOrder) = 0;
		virtual void OnRtnTrade(const  TradeUpdate& apTrade)=0;

	};
}
typedef  AT::IStrategy* (*CreateStrInstFun) (const char* aConfigFile, AT::IDriver_TD* apTradeSpi, AT::IStrategySpi* aStrSpi);
DLL_API  AT::IStrategy* CreateStr(const char*  aConfigFile, AT::IDriver_TD* apTradeSpi ,AT::IStrategySpi* aStrSpi);