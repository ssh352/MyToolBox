#include "ExecutorFactory.h"

#include "LimitToCancelExecutor.h"
#include "FollowExecutor.h"
#include "MarketExecutor.h"
#include "LimitExecutor.h"
#include "PairExecutor.h"
#include "CloseExecutor_3Level.h"
#include "StopLossExecutor.h"
#include "LimitFollowCancelExecutor.h"
#include "ATLogger.h"
namespace AT
{


 boost::shared_ptr<IExecutor> ExecutorFactory::CreateExecutor( const std::string& aExecutorType, const std::string& aConfigFile )
{
	boost::property_tree::ptree lConfigPtree;
	read_xml(aConfigFile,lConfigPtree);
	return CreateExecutor(aExecutorType,lConfigPtree);
}

 boost::shared_ptr<IExecutor> ExecutorFactory::CreateExecutor( const std::string& aExecutorType, const boost::property_tree::ptree& aConfigPtree )
 {
	 boost::shared_ptr<IExecutor> lret;
	 if( "Limit" == aExecutorType)
	 {
		 lret.reset(new AT::LimitExecutor(aConfigPtree));
	 }
	 else if ("Market" == aExecutorType)
	 {
		 lret.reset(new AT::MarketExecutor(aConfigPtree));
	 } 
	 else if ("LimitToCancel" == aExecutorType)
	 {
		 lret.reset(new AT::LimitToCancelExecutor(aConfigPtree));
	 }
	 else if("Follow" == aExecutorType)
	 {
		 lret.reset(new AT::FollowExecutor(aConfigPtree));
	 }
	 else if("Pair" == aExecutorType)
	 {
		 lret.reset(new AT::PairExecutor(aConfigPtree));
	 }
	 else if("Close" == aExecutorType)
	 {
		 lret.reset(new AT::CloseExecutor_3Level(aConfigPtree));	
	 }
	 else if("StopLoss" == aExecutorType)
	 {
		 lret.reset(new AT::StopLossExecutor(aConfigPtree));
	 }
	 else if("LimitFollowCancel" == aExecutorType)
	 {
		 lret.reset(new AT::LimitFollowCancelExecutor(aConfigPtree));
	 }

	 if(!lret)
	 {
		 ATLOG(L_ERROR,"Did not Crete Executor for ExecutorType");
		 ATLOG(L_ERROR,aExecutorType);
	 }
	 return lret;
 }

}