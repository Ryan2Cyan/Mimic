#include "Logger.h"
#include <iostream>

namespace Mimic
{
	std::shared_ptr<Logger> Logger::_coreLogger;

	void Logger::Init()
	{
		_coreLogger = std::make_shared<Logger>();
	}

	void Logger::DebugLog(const std::string& message)
	{
		std::cout << message << std::endl;
	}
}