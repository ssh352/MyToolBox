#include <C:\GitTrunk\MyToolBox\AT\AT_Driver\ATLogger.h>

#if 0

#include <string>
namespace AT
{

	enum  LogLevel
	{
		L_DEBUG ,
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
void InitLog();
void   ATLOG(AT::LogLevel aLevel,const std::string& aStringForLog);


#include <iostream>
#include <boost/locale/generator.hpp>
#include <boost/date_time/posix_time/posix_time_types.hpp>

#include <boost/log/common.hpp>
#include <boost/log/expressions.hpp>
#include <boost/log/utility/setup/file.hpp>
#include <boost/log/utility/setup/console.hpp>
#include <boost/log/utility/setup/common_attributes.hpp>
#include <boost/log/sources/logger.hpp>
#include <boost/log/support/date_time.hpp>
#include <boost/log/attributes/current_process_name.hpp>

namespace logging = boost::log;
namespace sinks = boost::log::sinks;
namespace attrs = boost::log::attributes;
namespace src = boost::log::sources;
namespace expr = boost::log::expressions;
namespace keywords = boost::log::keywords;




template< typename CharT, typename TraitsT >
inline std::basic_ostream< CharT, TraitsT >& operator<< (
	std::basic_ostream< CharT, TraitsT >& strm,  AT::LogLevel lvl)
{



	switch (lvl)
	{
	case LogLevel::L_DEBUG:
		strm <<"DEBUG";
		break;
	case LogLevel::L_ERROR:
		strm << "ERROR";
		break;
	case LogLevel::L_INFO:
		strm << "INFO";
		break;
	case LogLevel::L_WARN:
		strm<<"WARN";
		break;
	default:
		break;
	}


	return strm;
}
//]




//[ example_wide_char_logging_initialization
// Declare attribute keywords
BOOST_LOG_ATTRIBUTE_KEYWORD(severity, "Severity", AT::LogLevel)
	BOOST_LOG_ATTRIBUTE_KEYWORD(timestamp, "TimeStamp", boost::posix_time::ptime)

	void init_logging()
{
	const std::string moudleName = boost::log::aux::get_process_name();
	boost::shared_ptr< sinks::synchronous_sink< sinks::text_file_backend > > sink = logging::add_file_log
		(
		keywords::open_mode = std::ios_base::app,
		keywords::file_name = moudleName +"_%Y_%m_%d_%N.log",                                        /*< file name pattern >*/
		keywords::rotation_size = 10 * 1024 * 1024,                                   /*< rotate files every 10 MiB... >*/
		keywords::time_based_rotation = sinks::file::rotation_at_time_point(0, 0, 0),/*< ...or at midnight >*/
		keywords::format = expr::stream
		<< expr::format_date_time(timestamp, "%H:%M:%S.%f")
		<<" ["<< expr::attr< boost::log::attributes::current_process_id::value_type>("ProcessID")<<"]"
		<<" ["<< expr::attr< boost::log::attributes::current_thread_id::value_type>("ThreadID") <<"]" 
		<< " ["<<expr::attr< unsigned int >("LineID")<<"]"
		<< " [" << severity << "] "
		<< expr::message
		);
	

}


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



void ATLOG( AT::LogLevel aLevel,const std::string& aStringForLog )
{
	src::severity_logger<  AT::LogLevel > slg;
	BOOST_LOG_SEV(slg,aLevel) <<"TEst";
	BOOST_LOG_SEV(slg,aLevel) <<aStringForLog;

}

void InitLog()
{
	init_logging();
}



#endif



int main()
{
	ATLOG(AT::L_ERROR,"what the problem");
	system("pause");
	return 0;
}