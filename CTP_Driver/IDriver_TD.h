#pragma once

#include <string>

#include "CTP_API.h"
#include <map>

// 可以使用Text文本作为参数  将解析工作放到Driver内部 以此来保证接口的统一


namespace AT
{
	enum AT_ERROR_CODE
	{
		
	};

	class IDriver_TD
	{
		
	public:
		virtual std::string CreateOrder(OrderTypePtr aNewOrder);
		virtual void DeleteOrder(const std::string& aClientOrderID);
		virtual	void ModifyOrder(const std::string& aRequest);
		virtual void QueryPosition(const std::string& aRequest);

	public:
		virtual Init(std::map<std::string,std::string> aConfigMap);
	};




	class ITradeSpi
	{
	public:
		ITradeSpi();
		virtual ~ITradeSpi();

	public:
		virtual void OnRtnErron(int aErrorCode,std::string aErrorMsg);
		virtual void OnRtnOrder(OrderTypePtr apOrder);
		virtual void OnRtnTrade(TradeTypePtr apTrade);
		virtual void OnRtnPosition(const std::string& aRet)
	};
}

