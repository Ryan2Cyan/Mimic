#include "Logger.h"
#include <GL/glew.h>

namespace MimicUtility
{
	std::shared_ptr<Logger> Logger::_coreLogger;
	void Logger::Init()
	{
		_coreLogger = std::make_shared<Logger>();
	}

    void Logger::DebugLog(const char* format)
    {
#ifdef MIMIC_DEBUG
        Log("[DEBUG] ");
        std::cout << format << std::endl;
#endif // MIMIC_DEBUG
    }

    void Logger::LogInfo(const char* format)
    {
#ifdef MIMIC_DEBUG
        Log("\x1b[36m[INFO] ");
        std::cout << format << std::endl;
        Log("\x1b[0m");
#endif
    }

    void Logger::LogWarning(const char* format)
    {
        Log("\x1b[1m\033[33m[WARNING] ");
        std::cout << format << std::endl;
        Log("\033[0;37m\x1b[0m");
    }

    void Logger::LogError(const char* format)
    {
        Log("\x1b[1m\x1b[31m[ERROR] ");
        std::cout << format << std::endl;
    }

    void Logger::LogFatal(const char* format)
    {
        Log("\x1b[1m\x1b[31m[FATAL] ");
        std::cout << format << std::endl;
    }

    void Logger::Log(const char* format)
    { 
        std::cout << format; 
    }

    // source: https://learnopengl.com/In-Practice/Debugging
	void Logger::LogOpenGLError(const std::string& callerName) const noexcept
	{
        GLenum errorCode;
        while ((errorCode = glGetError()) != GL_NO_ERROR)
        {
            std::string error;
            switch (errorCode)
            {
            case GL_INVALID_ENUM: error = "INVALID_ENUM"; break;
            case GL_INVALID_VALUE: error = "INVALID_VALUE"; break;
            case GL_INVALID_OPERATION: error = "INVALID_OPERATION"; break;
            case GL_STACK_OVERFLOW: error = "STACK_OVERFLOW"; break;
            case GL_STACK_UNDERFLOW: error = "STACK_UNDERFLOW"; break;
            case GL_OUT_OF_MEMORY: error = "OUT_OF_MEMORY"; break;
            case GL_INVALID_FRAMEBUFFER_OPERATION: error = "INVALID_FRAMEBUFFER_OPERATION"; break;
            }
            MIMIC_LOG_WARNING("[Mimic::%] OpenGL error: %", callerName, error);
        }
	}
}