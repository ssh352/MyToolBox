#pragma once
#include <string>
namespace AT
{

	enum  class LogLevel
	{
		L_DEBUG,
		L_INFO,
		L_WARN,
		L_ERROR,
	};
	class ATLogger
	{
	public:
		ATLogger(void);
		~ATLogger(void);
	public:
		void log(LogLevel,const char* format,...);
		void log(LogLevel,const std::string& aStringForLog);
	};

}

void ATLOG(AT::LogLevel aLevel,const std::string& aStringForLog);

