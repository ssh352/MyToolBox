#pragma  once

#include <string>
class IClientOrderIDGenerator
{
	virtual std::string GeneratorClientID() = 0;
};

