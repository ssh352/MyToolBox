#pragma once
#include <string>
#include <boost\shared_ptr.hpp>
namespace AT
{
	class IAccount;
	class IDriver_TD;
	class AccountFactory
	{
	public:
		static boost::shared_ptr<IAccount> CereateAccount(const std::string& AccountType, const std::string& AccountConfig, AT::IDriver_TD* apTD);
	};
}


