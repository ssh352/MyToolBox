#pragma once

#include <boost\shared_ptr.hpp>
namespace AT
{
class IExecutor;

class ExecutorFactory
{
public:
	static  boost::shared_ptr<IExecutor> CreateExecutor(const std::string& aExecuorType, const std::string& aConfigFile);
};

}