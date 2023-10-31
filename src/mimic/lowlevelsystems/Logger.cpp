#include "Logger.h"
#include <iostream>
#include <cstdio>

namespace Mimic
{
	std::shared_ptr<Logger> Logger::_coreLogger;

	void Logger::Init()
	{
		_coreLogger = std::make_shared<Logger>();
	}

	const int Logger::LogWarning(char const* const format, ...)
	{
		Log("\033[33m");
		Log("\n[Warning]: ");
		int result;
		va_list argList;
		__crt_va_start(argList, format);
		result = _vfprintf_l(stdout, format, NULL, argList);
		__crt_va_end(argList);
		Log("\033[0;37m");
		return result;
	}

	const int Logger::Log(char const* const format, ...)
	{
		int result;
		va_list argList;
		__crt_va_start(argList, format);
		result = _vfprintf_l(stdout, format, NULL, argList);
		__crt_va_end(argList);
		return result;
	}
}