#include "Shader.h"

namespace Mimic
{
	Shader::Shader(const std::string vertexShaderPath, const std::string fragmentShaderPath)
	{
		// read shaders:
		const char* vertexShaderText = ReadShaderFile(vertexShaderPath.c_str());
		const char* fragmentShaderText = ReadShaderFile(fragmentShaderPath.c_str());
		assert(vertexShaderText != nullptr);
		assert(fragmentShaderText != nullptr);

		// compile shaders:
		const unsigned int vertexShaderId = CompileShaderText(vertexShaderText, GL_VERTEX_SHADER);
		const unsigned int fragmentShaderId = CompileShaderText(fragmentShaderText, GL_FRAGMENT_SHADER);
		assert(vertexShaderId != 0);
		assert(fragmentShaderId != 0);

		// link shaders to program:
		ShaderProgramId = glCreateProgram();
		glAttachShader(ShaderProgramId, vertexShaderId);
		glAttachShader(ShaderProgramId, fragmentShaderId);
		glLinkProgram(ShaderProgramId);
		int success;
		char infoLog[512];
		glGetProgramiv(ShaderProgramId, GL_LINK_STATUS, &success);
		if (!success)
		{
			glGetProgramInfoLog(ShaderProgramId, 512, NULL, infoLog);
			std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
		}

		glDeleteShader(vertexShaderId);
		glDeleteShader(fragmentShaderId);
	}

	const unsigned int Shader::CompileShaderText(const char* const fileText, const unsigned int shaderType)
	{
		const unsigned int shaderId = glCreateShader(shaderType);
		glShaderSource(shaderId, 1, &fileText, NULL);
		glCompileShader(shaderId);
		int success;
		char infoLog[512];
		glGetShaderiv(shaderId, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			glGetShaderInfoLog(shaderId, 512, NULL, infoLog);
			std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
			return 0;
		}
		return shaderId;
	}

	const char* Shader::ReadShaderFile(const char* const fileName)
	{
		std::ifstream shaderFile(fileName);
		char* shaderText = nullptr;
		if (shaderFile.is_open())
		{
			shaderFile.seekg(0, shaderFile.end);
			int length = (int)shaderFile.tellg();
			shaderFile.seekg(0, shaderFile.beg);
			shaderText = new char[length];
			shaderFile.read(shaderText, length);
			if (!shaderFile.eof())
			{
				shaderFile.close();
				std::cerr << "WARNING: could not read shader from file: " << fileName << std::endl;
				return nullptr;
			}
			length = (int)shaderFile.gcount();
			shaderText[length - 1] = 0;
			shaderFile.close();
			return shaderText;
		}
		else
		{
			std::cerr << "WARNING: could not open shader from file: " << fileName << std::endl;
			return nullptr;
		}
	}

	std::shared_ptr<Shader> Shader::Initialise(const std::string vertexShaderPath, const std::string fragmentShaderPath) const
	{
		return std::make_shared<Shader>(vertexShaderPath, fragmentShaderPath);
	}

	void Shader::Activate()
	{
		glUseProgram(ShaderProgramId);
	}
	
	void Shader::SetBool(const char* name, const bool value) const
	{
		glUniform1i(glGetUniformLocation(ShaderProgramId, name), (int)value);
	}

	void Shader::SetInt(const char* name, const int value) const
	{
		glUniform1i(glGetUniformLocation(ShaderProgramId, name), value);
	}

	void Shader::SetFloat(const char* name, const float value) const
	{
		glUniform1f(glGetUniformLocation(ShaderProgramId, name), value);
	}
	
}