#include "ATLogger.h"
#include <mutex>
#include <iostream>
namespace AT
{
	ATLogger::ATLogger(void)
	{
	}


	ATLogger::~ATLogger(void)
	{
	}

	void ATLogger::log( LogLevel,const std::string& aStringForLog )
	{

	}

}
std::mutex g_IOlock;
void ATLOG( AT::LogLevel aLevel,const std::string& aStringForLog )
{
	g_IOlock.lock();
	std::cout<<aStringForLog<<"\n";
	g_IOlock.unlock();
}

