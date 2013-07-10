#include "ExecutorFactory.h"

#include "LimitToCancelExecutor.h"
#include "FollowExecutor.h"
#include "MarketExecutor.h"
#include "LimitExecutor.h"
#include "CloseExecutor_3Level.h"
#include "ATLogger.h"
namespace AT
{

 boost::shared_ptr<IExecutor> ExecutorFactory::CreateExecutor( const std::string& aExecutorType, const std::string& aConfigFile )
{

	boost::shared_ptr<IExecutor> lret;
	if( "Limit" == aExecutorType)
	{
		lret.reset(new AT::LimitExecutor(aConfigFile));
	}
	else if ("Market" == aExecutorType)
	{
		lret.reset(new AT::MarketExecutor(aConfigFile));
	} 
	else if ("LimitToCancel" == aExecutorType)
	{
		lret.reset(new AT::LimitToCancelExecutor(aConfigFile));
	}
	else if("Follow" == aExecutorType)
	{
		lret.reset(new AT::FollowExecutor(aConfigFile));
	}

	if(!lret)
	{
		ATLOG(L_ERROR,"Did not Crete Executor for ExecutorType");
		ATLOG(L_ERROR,aExecutorType);
	}
	return lret;
}

}