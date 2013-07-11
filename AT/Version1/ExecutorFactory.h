#pragma once

#include <boost\shared_ptr.hpp>
namespace AT
{
class IExecutor;


static const std::string LimitExecutorType = "Limit";
static const std::string MarketExecutorType = "Market";
static const std::string LimitToCancelType = "LimitToCancel";
static const std::string FollowType = "Follow";
static const std::string OpenClosePairType = "Pair";


class ExecutorFactory
{
public:
	static  boost::shared_ptr<IExecutor> CreateExecutor(const std::string& aExecuorType, const std::string& aConfigFile);

	////复制构造函数，避免每次去读文件
	//template <typename ExecutorType>
	//boost::shared_ptr<ExecutorType> CreateExecutor( boost::shared_ptr<IExecutor> aExecutorPtr)
	//{
	//	boost::shared_ptr<ExecutorType> lret;
	//	lret.reset(new ExecutorType(*aExecutorPtr));
	//	return lret;
	//}
};

}