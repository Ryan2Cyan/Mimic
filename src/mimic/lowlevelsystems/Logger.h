#include <string>
#include <memory>

// Source: https://www.youtube.com/watch?v=dZr-53LAlOw&t=1619s

#define MIMIC_DEBUG_LOG(...)		::Mimic::Logger::GetCoreLogger()->DebugLog(__VA_ARGS__)

namespace Mimic
{
	// #############################################################################
	// logger stuct:
	// #############################################################################
	struct Logger
	{
		static void Init();
		void DebugLog(const std::string& message);
		inline static std::shared_ptr<Logger>& GetCoreLogger() { return _coreLogger; }

	private:
		static std::shared_ptr<Logger> _coreLogger;
	};
}