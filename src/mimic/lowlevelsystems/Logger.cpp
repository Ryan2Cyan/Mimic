#include "Logger.h"
#include <iostream>
#include <cstdio>

namespace Mimic
{
	std::shared_ptr<Logger> Logger::_coreLogger;

	void Logger::NewLine()
	{
		_vfprintf_l(stdout, "\n", nullptr, nullptr);
	}

	void Logger::Init()
	{
		_coreLogger = std::make_shared<Logger>();
	}

	const int Logger::DebugLog(char const* const format, ...)
	{
		int result;
		va_list argList;
		__crt_va_start(argList, format);
		result = _vfprintf_l(stdout, format, nullptr, argList);
		__crt_va_end(argList);
		NewLine();
		return result;
	}

	const int Logger::LogWarning(char const* const format, ...)
	{
		LogInternal("\033[33m");
		LogInternal("[Warning]: ");
		int result;
		va_list argList;
		__crt_va_start(argList, format);
		result = _vfprintf_l(stdout, format, nullptr, argList);
		__crt_va_end(argList);
		LogInternal("\033[0;37m");
		NewLine();
		return result;
	}

	const int Logger::LogInternal(char const* const format, ...)
	{
		int result;
		va_list argList;
		__crt_va_start(argList, format);
		result = _vfprintf_l(stdout, format, nullptr, argList);
		__crt_va_end(argList);
		return result;
	}
}