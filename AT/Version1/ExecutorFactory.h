#pragma once

#include <boost\shared_ptr.hpp>
#include <boost\property_tree\ptree.hpp>
namespace AT
{
class IExecutor;


static const std::string LimitExecutorType = "Limit";
static const std::string MarketExecutorType = "Market";
static const std::string LimitToCancelType = "LimitToCancel";
static const std::string FollowExecutorType = "Follow";
static const std::string OpenClosePairType = "Pair";


class ExecutorFactory
{
public:
	static  boost::shared_ptr<IExecutor> CreateExecutor(const std::string& aExecuorType, const std::string& aConfigFile);
	static  boost::shared_ptr<IExecutor> CreateExecutor(const std::string& aExecuorType, const boost::property_tree::ptree& aConfigPtree);
};

}