#include "AccountFactory.h"
#include "Account.h"
#include "RecovedAccount.h"
namespace AT
{


	boost::shared_ptr<IAccount> AccountFactory::CereateAccount( const std::string& AccountType, const std::string& AccountConfig, AT::IDriver_TD* apTD )
	{
		boost::shared_ptr<IAccount> lret;
		if ("Normal" == AccountType)
		{
			lret.reset(new Account(AccountConfig,apTD));
		} 
		else if("Record" == AccountType)
		{
			lret.reset(new RecovedAccount(AccountConfig));
		}

		return lret;
	}

}