#pragma once

#include <string>

class ITradeSpi;

namespace AT
{
	//accept xml string for question

	//thread safe Driver promise it
	class IDriver_TD
	{

	public:
		virtual Init(const std::string&  aConfigStr,ITradeSpi* apTradeSpi);
	public:
		virtual std::string CreateOrder(const std::string& aNewOrder);
		virtual void DeleteOrder(const std::string& aClientOrderID);
		virtual	void ModifyOrder(const std::string& aRequest);
		virtual void QueryPosition(const std::string& aRequest);

	};

}

