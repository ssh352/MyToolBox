#pragma once

#include <string>
#include <map>
class ITradeSpi;

namespace AT
{
	//accept xml string for question

	//thread safe Driver promise it
	class IDriver_TD
	{

	public:
		virtual Init(const std::map<std::string,std::string>& aConfigMap,ITradeSpi* apTradeSpi);
	public:
		virtual std::string CreateOrder(const std::string& aNewOrder);
		virtual void DeleteOrder(const std::string& aClientOrderID);
		virtual	void ModifyOrder(const std::string& aRequest);
		virtual void QueryPosition(const std::string& aRequest);

	};

}

