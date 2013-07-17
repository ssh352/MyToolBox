#pragma once
#include <string>
namespace AT
{

	enum   LogLevel
	{
		L_DEBUG ,
		L_INFO,
		L_WARN,
		L_ERROR,
	};
}
void   ATLOG(AT::LogLevel aLevel,const std::string& aStringForLog);
void   ATLOG(AT::LogLevel aLevel,char const* apFormatString,...);


