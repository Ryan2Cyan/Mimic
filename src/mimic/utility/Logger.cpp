#include "Logger.h"

namespace Mimic
{
	std::shared_ptr<Logger> Logger::_coreLogger;
	void Logger::Init()
	{
		_coreLogger = std::make_shared<Logger>();
	}
}