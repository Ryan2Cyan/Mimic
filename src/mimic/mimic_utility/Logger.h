#pragma once
#include <memory>
#include <string>
#include <iostream>

// Source: https://www.youtube.com/watch?v=dZr-53LAlOw&t=1619s

#define MIMIC_DEBUG_LOG(...)			::MimicUtility::Logger::GetCoreLogger()->DebugLog(__VA_ARGS__)
#define MIMIC_LOG_INFO(...)				::MimicUtility::Logger::GetCoreLogger()->LogInfo(__VA_ARGS__)
#define MIMIC_LOG_WARNING(...)			::MimicUtility::Logger::GetCoreLogger()->LogWarning(__VA_ARGS__)
#define MIMIC_LOG_ERROR(...)			::MimicUtility::Logger::GetCoreLogger()->LogError(__VA_ARGS__)
#define MIMIC_LOG_FATAL(...)			::MimicUtility::Logger::GetCoreLogger()->LogFatal(__VA_ARGS__)
#define MIMIC_LOG(...)					::MimicUtility::Logger::GetCoreLogger()->Log(__VA_ARGS__)
#define MIMIC_LOG_OPENGL(...)			::MimicUtility::Logger::GetCoreLogger()->LogOpenGLError(__VA_ARGS__)

namespace MimicUtility
{
	/// <summary>
	/// Logging system. Allows for logging functionality, including debug, warning, error, and fatal messages
	/// to be displayed to the console.
	/// </summary>
	struct Logger
	{
		static void Init();

		template<typename T, typename... Targs> void DebugLog(const char* format, T value, Targs... Fargs)
		{
#ifdef MIMIC_DEBUG
			Log("[DEBUG] ");
			Log(format, value, Fargs...);
			Log("\n");
#endif		
		}

		template<typename T, typename... Targs> void LogInfo(const char* format, T value, Targs... Fargs)
		{
#ifdef MIMIC_DEBUG
			Log("\x1b[36m[INFO] ");
			Log(format, value, Fargs...);
			Log("\n\x1b[0m");
#endif
		}

		template<typename T, typename... Targs> void LogWarning(const char* format, T value, Targs... Fargs)
		{
			Log("\x1b[1m\033[33m[WARNING] ");
			Log(format, value, Fargs...);
			Log("\n\033[0;37m\x1b[0m");
		}

		template<typename T, typename... Targs> void LogError(const char* format, T value, Targs... Fargs)
		{
			Log("\x1b[1m\x1b[31m[ERROR] ");
			Log(format, value, Fargs...);
			Log("\n\033[0;37m\x1b[0m");
		}


		template<typename T, typename... Targs> void LogFatal(const char* format, T value, Targs... Fargs)
		{
			Log("\x1b[1m\x1b[31m[FATAL] ");
			Log(format, value, Fargs...);
			Log("\n\033[0;37m\x1b[0m");
		}

		template<typename T, typename... Targs> void Log(const char* format, T value, Targs... Fargs)
		{
			// loop through each char in format
			for (; *format != '\0'; format++)
			{
				// if the current char is '%', print the value inputted as a parameter, then recusive call:
				if (*format == '%') 
				{
					std::cout << value;
					Log(format + 1, Fargs...); // recursive call
					return;
				}
				// if not '%', then just print the current character:
				std::cout << *format;
			}
		}

		inline static std::shared_ptr<Logger>& GetCoreLogger() { return _coreLogger; }
		void LogOpenGLError(const std::string& callerName) const noexcept;
		void DebugLog(const char* format);
		void LogInfo(const char* format);
		void LogWarning(const char* format);
		void LogError(const char* format);
		void LogFatal(const char* format);
		void Log(const char* format);

	private:
		static std::shared_ptr<Logger> _coreLogger;
	};
}