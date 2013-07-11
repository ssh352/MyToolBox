#pragma once

#include <boost\shared_ptr.hpp>
namespace AT
{
class IExecutor;

class ExecutorFactory
{
public:
	static  boost::shared_ptr<IExecutor> CreateExecutor(const std::string& aExecuorType, const std::string& aConfigFile);

	////���ƹ��캯��������ÿ��ȥ���ļ�
	//template <typename ExecutorType>
	//boost::shared_ptr<ExecutorType> CreateExecutor( boost::shared_ptr<IExecutor> aExecutorPtr)
	//{
	//	boost::shared_ptr<ExecutorType> lret;
	//	lret.reset(new ExecutorType(*aExecutorPtr));
	//	return lret;
	//}
};

}