#pragma once
#include <memory>
#include <iostream>

// Source: https://www.youtube.com/watch?v=dZr-53LAlOw&t=1619s

#define MIMIC_DEBUG_LOG(...)			::Mimic::Logger::GetCoreLogger()->DebugLog(__VA_ARGS__)
#define MIMIC_LOG_INFO(...)				::Mimic::Logger::GetCoreLogger()->LogInfo(__VA_ARGS__)
#define MIMIC_LOG_WARNING(...)			::Mimic::Logger::GetCoreLogger()->LogWarning(__VA_ARGS__)
#define MIMIC_LOG_ERROR(...)			::Mimic::Logger::GetCoreLogger()->LogError(__VA_ARGS__)
#define MIMIC_LOG_FATAL(...)			::Mimic::Logger::GetCoreLogger()->LogFatal(__VA_ARGS__)

namespace Mimic
{
	// #############################################################################
	// logger stuct:
	// #############################################################################
	struct Logger
	{
		static void Init();

		void DebugLog(const char* format)
		{
#ifdef MIMIC_DEBUG
			Log("[DEBUG] ");
			std::cout << format << std::endl;
#endif // MIMIC_DEBUG
		}

		template<typename T, typename... Targs> void DebugLog(const char* format, T value, Targs... Fargs)
		{
#ifdef MIMIC_DEBUG
			Log("[DEBUG] ");
			Log(format, value, Fargs...);
			Log("\n");
#endif		
		}

		void LogInfo(const char* format) 
		{ 
#ifdef MIMIC_DEBUG
			Log("\x1b[36m[INFO] ");
			std::cout << format << std::endl; 
			Log("\x1b[0m");
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

		void LogWarning(const char* format) 
		{
			Log("\x1b[1m\033[33m[WARNING] ");
			std::cout << format << std::endl;
		}

		template<typename T, typename... Targs> void LogWarning(const char* format, T value, Targs... Fargs)
		{
			Log("\x1b[1m\033[33m[WARNING] ");
			Log(format, value, Fargs...);
			Log("\n\033[0;37m\x1b[0m");
		}

		void LogError(const char* format) 
		{ 
			Log("\x1b[1m\x1b[31m[ERROR] ");
			std::cout << format << std::endl;
		}

		template<typename T, typename... Targs> void LogError(const char* format, T value, Targs... Fargs)
		{
			Log("\x1b[1m\x1b[31m[ERROR] ");
			Log(format, value, Fargs...);
			Log("\n\033[0;37m\x1b[0m");
		}

		void LogFatal(const char* format) 
		{ 
			Log("\x1b[1m\x1b[31m[FATAL] ");
			std::cout << format << std::endl;
		}

		template<typename T, typename... Targs> void LogFatal(const char* format, T value, Targs... Fargs)
		{
			Log("\x1b[1m\x1b[31m[FATAL] ");
			Log(format, value, Fargs...);
			Log("\n\033[0;37m\x1b[0m");
		}


		void Log(const char* format) { std::cout << format; }

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

	private:
		static std::shared_ptr<Logger> _coreLogger;
	};
}