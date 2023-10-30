#include "Shader.h"
#include <cassert>
#include <iostream>
#include <fstream>
#include <array>
#include <glm/gtc/type_ptr.hpp>

// Source: https://www.youtube.com/watch?v=8wFEzIYRZXg

namespace Mimic
{
	void Shader::Load(const std::string& path)
	{
		const std::string sourceCode = ReadShaderFile(path);
		auto shaderSources = PreProcess(sourceCode);
		CompileShaderText(shaderSources);
	}

	const std::string Shader::ReadShaderFile(const std::string& path)
	{
		std::string result;
		std::ifstream in(path, std::ios::in | std::ios::binary); // in = readonly, binary = reading bytes mode
		if (in)
		{
			in.seekg(0, std::ios::end);
			result.resize(in.tellg());
			in.seekg(0, std::ios::beg);
			in.read(&result[0], result.size());
			in.close();
		}
		else
		{
			// error, could not read shader file:
			std::cerr << "WARNING: could not open shader from file: " << path << "." << std::endl;
			return nullptr;
		}
		return result;
	}

	GLenum Shader::ShaderTypeFromString(const std::string& type)
	{
		if (type == "vertex") return GL_VERTEX_SHADER;
		if (type == "fragment" || type == "pixel") return GL_FRAGMENT_SHADER;

		std::cerr << "Unknown shader type: " << type << std::endl;
		return 0;	
	}

	std::unordered_map<GLuint, std::string> Shader::PreProcess(const std::string& source)
	{
		// load all different shader types from one source file:
		std::unordered_map<GLenum, std::string> shaderSources;

		const char* typeToken = "#type";
		size_t typeTokenLength = strlen(typeToken);
		size_t currentPosition = source.find(typeToken, 0);
		while (currentPosition != std::string::npos)
		{
			// find current type specifier and store it:
			size_t endOfLinePosition = source.find_first_of("\r\n", currentPosition);
			assert(endOfLinePosition != std::string::npos);
			size_t begin = currentPosition + typeTokenLength + 1;
			std::string type = source.substr(begin, endOfLinePosition - begin);
			assert(type == "vertex" || type == "fragment" || type == "pixel");

			// store source code associated with the shader type:
			size_t nextLinePosition = source.find_first_not_of("\r\n", endOfLinePosition);
			currentPosition = source.find(typeToken, nextLinePosition);
			shaderSources[ShaderTypeFromString(type)] = source.substr(nextLinePosition, currentPosition - (nextLinePosition == std::string::npos ? source.size() - 1 : nextLinePosition));
		}
		return shaderSources;
	}
	
	void Shader::CompileShaderText(const std::unordered_map<GLenum, std::string>& shaderSources)
	{
		GLuint programId = glCreateProgram();

		const int numberOfShaders = shaderSources.size();
		constexpr int maxShadersSupported = 2;
		assert(shaderSources.size() <= maxShadersSupported);
		std::array<GLenum, maxShadersSupported> glShaderIds;
		int glShaderIdIndex = 0;
		
		// generate & attach each shader to the program:
		for (auto& shader : shaderSources)
		{
			GLenum shaderType = shader.first;
			const std::string& shaderSource = shader.second;

			const unsigned int shaderId = glCreateShader(shaderType);
			const char* const shaderSourceCStr = shaderSource.c_str();
			glShaderSource(shaderId, 1, &shaderSourceCStr, NULL);
			glCompileShader(shaderId);
			int success;
			char infoLog[512];
			glGetShaderiv(shaderId, GL_COMPILE_STATUS, &success);
			if (!success)
			{
				glGetShaderInfoLog(shaderId, 512, NULL, infoLog);
				std::cout << "ERROR: Shader has failed to compile.\n" << infoLog << std::endl;
				assert(false);
			}

			glAttachShader(programId, shaderId);
			glShaderIds[glShaderIdIndex] = shaderId;
			glShaderIdIndex++;
		}

		// link shaders to program:
		glLinkProgram(programId);
		int success;
		char infoLog[512];
		glGetProgramiv(programId, GL_LINK_STATUS, &success);
		if (!success)
		{
			glGetProgramInfoLog(programId, 512, NULL, infoLog);
			std::cout << "ERROR: Shader(s) have failed to link to the ShaderProgram.\n" << infoLog << std::endl;

			glDeleteProgram(programId);
			for (auto shaderId : glShaderIds) glDeleteShader(shaderId);
			return;
		}

		ShaderProgramId = programId;
		// assign uniforms:
		_modelMatrixUniformLocation = glGetUniformLocation(ShaderProgramId, "u_Model");
		_viewMatrixUniformLocation = glGetUniformLocation(ShaderProgramId, "u_View");
		_projectionMatrixUniformLocation = glGetUniformLocation(ShaderProgramId, "u_Projection");

		for (auto shaderId : glShaderIds) glDetachShader(ShaderProgramId, shaderId);
	}

	void Shader::SetBool(const char* name, const bool value) const
	{
		GLint location = glGetUniformLocation(ShaderProgramId, name);
		if (location == -1) std::cerr << "WARNING: Could not find location of shader uniform [bool]: " << name << std::endl;
		glUniform1i(location, (int)value);
	}

	void Shader::SetInt(const char* name, const int value) const
	{
		GLint location = glGetUniformLocation(ShaderProgramId, name);
		if (location == -1) std::cerr << "WARNING: Could not find location of shader uniform [integer]: " << name << std::endl;
		glUniform1i(location, value);
	}

	void Shader::SetFloat(const char* name, const float value) const
	{
		GLint location = glGetUniformLocation(ShaderProgramId, name);
		if (location == -1) std::cerr << "WARNING: Could not find location of shader uniform [float]: " << name << std::endl;
		glUniform1f(location, value);
	}

	void Shader::SetMat4(const char* name, const glm::mat4 value) const
	{
		GLint location = glGetUniformLocation(ShaderProgramId, name);
		if(location == -1) std::cerr << "WARNING: Could not find location of shader uniform [mat4]: " << name << std::endl;
		glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(value));
	}

	void Shader::SetModelMatrix(const glm::mat4 value)
	{
		glUniformMatrix4fv(_modelMatrixUniformLocation, 1, GL_FALSE, glm::value_ptr(value));
	}

	void Shader::SetViewMatrix(const glm::mat4 value)
	{
		glUniformMatrix4fv(_viewMatrixUniformLocation, 1, GL_FALSE, glm::value_ptr(value));
	}

	void Shader::SetProjectionMatrix(const glm::mat4 value)
	{
		glUniformMatrix4fv(_projectionMatrixUniformLocation, 1, GL_FALSE, glm::value_ptr(value));
	}
	
}