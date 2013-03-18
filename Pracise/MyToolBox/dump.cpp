
#include <cstdio>
#include <Windows.h>
#include <DbgHelp.h>
#include "Dump.h"
#pragma comment(lib,"dbghelp.lib")

namespace MY_TOOL_BOX
{


	//MINIDUMPWRITEDUMP
	static void	write_dump_file(struct _EXCEPTION_POINTERS* exceptionInfo)
	{

		char working_path[256];
		GetModuleFileName(NULL, working_path, 256);
	

		// write the dump file in the log directory

		SYSTEMTIME systemTime;
		::GetLocalTime(&systemTime);

		sprintf(working_path, "crashdump_%4.4d%2.2d%2.2d-%2.2d%2.2d%2.2d.dmp",
			systemTime.wYear,
			systemTime.wMonth,
			systemTime.wDay,
			systemTime.wHour,
			systemTime.wMinute,
			systemTime.wSecond );

		HANDLE fileHandle = ::CreateFile(working_path, 
			GENERIC_WRITE, 
			FILE_SHARE_WRITE,
			NULL, 
			CREATE_ALWAYS, 
			FILE_ATTRIBUTE_NORMAL, 
			NULL);

		if (fileHandle == INVALID_HANDLE_VALUE)
		{
			return;
		}

		_MINIDUMP_EXCEPTION_INFORMATION ExInfo;
		ExInfo.ThreadId = ::GetCurrentThreadId();
		ExInfo.ExceptionPointers = exceptionInfo;
		ExInfo.ClientPointers = NULL;

		MiniDumpWriteDump(GetCurrentProcess(), GetCurrentProcessId(), fileHandle, MiniDumpNormal, &ExInfo, NULL, NULL);
		::CloseHandle(fileHandle);
	}

	static LPTOP_LEVEL_EXCEPTION_FILTER g_OldFilter(NULL);

	LONG WINAPI 
		unhandled_exception_hook(struct _EXCEPTION_POINTERS* exceptionInfo)
	{


		try 
		{
			write_dump_file(exceptionInfo);

		}
		catch (...) {}

		if(NULL != g_OldFilter)
		{
			return g_OldFilter(exceptionInfo);
		}
		else
		{
			//return EXCEPTION_CONTINUE_SEARCH;
			return EXCEPTION_EXECUTE_HANDLER;
		}
	}


	void RegisterCrashDumpExporter()
	{
		if(NULL == g_OldFilter)
		{
			g_OldFilter = ::SetUnhandledExceptionFilter(&unhandled_exception_hook);
			if(&unhandled_exception_hook == g_OldFilter)
				g_OldFilter = NULL;
		}
	}

	void UnregisterCrashDumpExporter()
	{
		::SetUnhandledExceptionFilter(g_OldFilter);
	}


}
