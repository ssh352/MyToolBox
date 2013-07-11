#pragma once

#include <boost\shared_ptr.hpp>
namespace AT
{
class IExecutor;

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