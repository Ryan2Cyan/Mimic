#include <string>
#include <memory>


// Source: https://www.youtube.com/watch?v=dZr-53LAlOw&t=1619s

#define MIMIC_LOG(...)				::Mimic::Logger::GetCoreLogger()->DebugLog(__VA_ARGS__)
#define MIMIC_LOG_WARNING(...)		::Mimic::Logger::GetCoreLogger()->LogWarning(__VA_ARGS__)
#define MIMIC_LOG_EROOR(...)		::Mimic::Logger::GetCoreLogger()->LogWarning(__VA_ARGS__)

namespace Mimic
{
	// #############################################################################
	// logger stuct:
	// #############################################################################
	struct Logger
	{
		static void Init();
		const int LogError(char const* const format, ...);
		const int LogWarning(char const* const format, ...);
		const int DebugLog(char const* const format, ...);
		void NewLine();
		inline static std::shared_ptr<Logger>& GetCoreLogger() { return _coreLogger; }

	private:
		const int LogInternal(char const* const format, ...);
		static std::shared_ptr<Logger> _coreLogger;
	};
}