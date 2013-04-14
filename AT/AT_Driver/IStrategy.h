#pragma once
#include <string>

#include "IDriver_TD.h"

namespace AT
{

	enum  StrategyState
	{
		Str_Init,//include sync
		Str_Start,
		Str_Stop,
		Str_Reload
	};


	class IStrategy 
	{
	public:
		IStrategy(IDriver_TD* apTD)
			:m_pTD(apTD)
		{

		}
		virtual ~IStrategy(void) {};

	public:
		virtual void NotifyState(const std::string& aErrorCode) = 0;
		virtual void SetStrategyPram(const std::string& apStrParam) = 0;
	public:
		virtual void OnMarketDepth(const std::string& aMarketDepth)= 0;
		virtual void OnRtnOrder(const std::string& apOrder) = 0;
		virtual void OnRtnTrade(const std::string& apTrade)=0;
		virtual void OnRtnPos(const std::string& aPos) = 0;

		virtual void Start() = 0;
		virtual void Stop() = 0;


	protected:
		IDriver_TD* m_pTD;
	};
}
typedef  AT::IStrategy* (*CreateStrFun) ( AT::IDriver_TD* apTradeSpi);
DLL_API  AT::IStrategy* CreateStr( AT::IDriver_TD* apTradeSpi);