#include <string>
#include <memory>


// Source: https://www.youtube.com/watch?v=dZr-53LAlOw&t=1619s

#define MIMIC_LOG(...)				::Mimic::Logger::GetCoreLogger()->Log(__VA_ARGS__)
#define MIMIC_LOG_WARNING(...)		::Mimic::Logger::GetCoreLogger()->LogWarning(__VA_ARGS__)

namespace Mimic
{
	// #############################################################################
	// logger stuct:
	// #############################################################################
	struct Logger
	{
		static void Init();
		const int LogWarning(char const* const format, ...);
		const int Log(char const* const format, ...);
		inline static std::shared_ptr<Logger>& GetCoreLogger() { return _coreLogger; }

	private:
		static std::shared_ptr<Logger> _coreLogger;
	};
}