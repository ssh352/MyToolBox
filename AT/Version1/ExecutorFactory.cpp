#include "ExecutorFactory.h"

#include "OpenFollowExecutor.h"
#include "MarketExecutor.h"
#include "LimitExecutor.h"
#include "CloseExecutor_3Level.h"
#include "ATLogger.h"
namespace AT
{

 boost::shared_ptr<IExecutor> ExecutorFactory::CreateExecutor( const std::string& aExecutorType, const std::string& aConfigFile )
{
	boost::shared_ptr<IExecutor> lret;
	//if( "OpenFollow" == aExecutorType)
	//{
	//	lret.reset(new AT::OpenFollowExecutor(aConfigFile));
	//}
	//else if ("OpenLimit" == aExecutorType)
	//{
	//	lret.reset(new OpenLimitExecutor(aConfigFile));
	//} 
	//else if ("OpenMarket" == aExecutorType)
	//{
	//	lret.reset(new OpenMarketExecutor(aConfigFile));
	//}
	//else if("Close3Level" == aExecutorType)
	//{
	//	lret.reset(new CloseExecutor_3Level(aConfigFile));
	//}

	if(!lret)
	{
		ATLOG(L_ERROR,"Did not Crete Executor for ExecutorType");
		ATLOG(L_ERROR,aExecutorType);
	}
	return lret;
}

}