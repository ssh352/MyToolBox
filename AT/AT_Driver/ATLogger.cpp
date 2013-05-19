#include "ATLogger.h"

//#include <boost/thread/once.hpp>
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




inline std::ostream & operator<< ( std::ostream & strm,  AT::LogLevel lvl)
{
	switch (lvl)
	{
	case AT::L_DEBUG:
		strm <<"DEBUG";
		break;
	case AT::L_ERROR:
		strm << "ERROR";
		break;
	case AT::L_INFO:
		strm << "INFO";
		break;
	case AT::L_WARN:
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
		keywords::time_based_rotation = sinks::file::rotation_at_time_point(0, 0, 0)/*< ...or at midnight >*/
		,keywords::format = expr::stream
		<< expr::format_date_time(timestamp, "%H:%M:%S.%f")
		<<" ["<< expr::attr< boost::log::aux::process::id>("ProcessID")<<"]"
		<<" ["<< expr::attr< boost::log::aux::thread::id>("ThreadID") <<"]" 
		<< " [LogLevel:" <<  expr::attr< AT::LogLevel >("Severity") << "] "
		<< expr::message
		);
	logging::add_common_attributes();

	src::severity_logger<  AT::LogLevel > slg;
	BOOST_LOG_SEV(slg,AT::L_INFO) <<"Log Start";

}
#include <mutex>
std::once_flag flag;
void InitOnce()
{
	//boost::call_once(once, init_logging);
	std::call_once(flag, init_logging );
	//init_logging();
}

void ATLOG( AT::LogLevel aLevel,const std::string& aStringForLog )
{
	InitOnce();
	static src::severity_logger<  AT::LogLevel > slg;
	BOOST_LOG_SEV(slg,aLevel) <<aStringForLog;
}

