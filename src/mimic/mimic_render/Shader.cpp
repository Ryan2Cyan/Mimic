#include "Shader.h"
#include <mimic_render/Texture.h>
#include <mimic_utility/Logger.h>
#include <fstream>
#include <array>
#include <glm/gtc/type_ptr.hpp>

// Source: https://www.youtube.com/watch?v=8wFEzIYRZXg

namespace MimicRender
{
	std::shared_ptr<Shader> Shader::Initialise(const std::string& path)
	{
		// Read the shader source file, then read and cache the shader's source code:
		const std::string sourceCode = ReadShaderFile(path);
		if (sourceCode.empty())
		{
			MIMIC_LOG_WARNING("[MimicRender::Shader] Initialisation unsuccessful, could not load shader from path \"%\"", path);
			return nullptr;
		}

		// Separate out the single source code into multiple pieces of data for each shader 
		// type (e.g. fragment or vertex):
		auto shaderSources = PreProcess(sourceCode);

		// Perform all OpenGL operations on the shader to generate a shader program and
		// attach and link shaders to it.
		const std::shared_ptr<Shader> shader = CompileShaderText(shaderSources);

		if (shader == nullptr)
		{
			MIMIC_LOG_WARNING("[MimicRender::Shader] Initialisation unsuccessful, could not pre-process shader code from path \"%\"", path);
			return nullptr;
		}
		
		shader->_initialised = true;
		return shader;
	}

	void Shader::UseShader() const noexcept
	{
		if (!_initialised) return;
		glUseProgram(_shaderProgramId);
	}

	std::string Shader::ReadShaderFile(const std::string& path)
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
		else MIMIC_LOG_WARNING("[MimicRender::Shader] Could not open shader from filepath: %", path);
	
		return result;
	}

	GLenum Shader::ShaderTypeFromString(const std::string& type)
	{
		if (type == "vertex") return GL_VERTEX_SHADER;
		if (type == "fragment" || type == "pixel") return GL_FRAGMENT_SHADER;

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
			
			// store source code associated with the shader type:
			size_t nextLinePosition = source.find_first_not_of("\r\n", endOfLinePosition);
			currentPosition = source.find(typeToken, nextLinePosition);
			const GLenum glEnumType = ShaderTypeFromString(type);
			if (glEnumType == 0)
			{
				MIMIC_LOG_WARNING("[MimicRender::Shader] Shader source type token \"%\" is invalid", type);
				return shaderSources;
			}
			shaderSources[ShaderTypeFromString(type)] = source.substr(nextLinePosition, currentPosition - (nextLinePosition == std::string::npos ? source.size() - 1 : nextLinePosition));
		}
		return shaderSources;
	}
	
	std::shared_ptr<Shader> Shader::CompileShaderText(const std::unordered_map<GLenum, std::string>& shaderSources)
	{
		std::shared_ptr<Shader> shader = std::make_shared<Shader>();

		// Generate shader program:
		GLuint programId = glCreateProgram();

		const int numberOfShaders = shaderSources.size();
		constexpr int maxShadersSupported = 2;
		assert(shaderSources.size() <= maxShadersSupported);
		std::array<GLenum, maxShadersSupported> glShaderIds;
		int glShaderIdIndex = 0;
		
		// Generate & attach each shader to the shader program:
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
			if (success != GL_TRUE)
			{
				glGetShaderInfoLog(shaderId, 512, NULL, infoLog);
				MIMIC_LOG_WARNING("[MimicRender::Shader] Shader(s) have failed to compile: %", infoLog);
				return nullptr;
			}

			glAttachShader(programId, shaderId);
			glShaderIds[glShaderIdIndex] = shaderId;
			glShaderIdIndex++;
		}

		// Link shaders to program:
		glLinkProgram(programId);
		int success;
		char infoLog[512];
		glGetProgramiv(programId, GL_LINK_STATUS, &success);
		if (!success)
		{
			glGetProgramInfoLog(programId, 512, NULL, infoLog);
			MIMIC_LOG_WARNING("[MimicRender::Shader] Shader(s) have failed to link to the ShaderProgram: %", infoLog);

			glDeleteProgram(programId);
			for (auto shaderId : glShaderIds) glDeleteShader(shaderId);
			return nullptr;
		}

		// Capture key uniform locations:
		shader->_modelMatrixUniformLocation = glGetUniformLocation(programId, "u_Model");
		shader->_viewMatrixUniformLocation = glGetUniformLocation(programId, "u_View");
		shader->_projectionMatrixUniformLocation = glGetUniformLocation(programId, "u_Projection");
		shader->_cameraPositionUniformLocation = glGetUniformLocation(programId, "u_CameraPosition");

		for (auto shaderId : glShaderIds) glDetachShader(programId, shaderId);

		shader->_shaderProgramId = programId;
		shader->_initialised = true;
		return shader;
	}

	void Shader::SetBool(const char* name, const bool value) const noexcept
	{
		if (!_initialised) return;
		GLint location = glGetUniformLocation(_shaderProgramId, name);
		if (location == -1)
		{
			MIMIC_LOG_WARNING("[MimicRender::Shader] Could not find location of shader uniform [bool] named \"%\"", name);
			return;
		}
		glUniform1i(location, (int)value);
	}

	void Shader::SetInt(const char* name, const int value) const noexcept
	{
		if (!_initialised) return;
		GLint location = glGetUniformLocation(_shaderProgramId, name);
		if (location == -1)
		{
			MIMIC_LOG_WARNING("[MimicRender::Shader] Could not find location of shader uniform [integer] named \"%\"", name);
			return;
		}
		glUniform1i(location, value);
	}

	void Shader::SetTexture(const char* name, const int& textureId, const int& bindPoint, const std::uint32_t& textureParams)
	{
		if (!_initialised) return;;
		SetInt(name, bindPoint);
		glActiveTexture(GL_TEXTURE0 + bindPoint);
		glBindTexture(Texture::GetGLTarget(textureParams), textureId);
	}

	void Shader::SetFloat(const char* name, const float value) const noexcept
	{
		if (!_initialised) return;
		GLint location = glGetUniformLocation(_shaderProgramId, name);
		if (location == -1)
		{
			MIMIC_LOG_WARNING("[MimicRender::Shader] Could not find location of shader uniform [float] named \"%\"", name);
			return;
		}
		glUniform1f(location, value);
	}

	void Shader::SetVector3(const char* name, const glm::vec3 value) const noexcept
	{
		if (!_initialised) return;
		GLint location = glGetUniformLocation(_shaderProgramId, name);
		if (location == -1)
		{
			MIMIC_LOG_WARNING("[MimicRender::Shader] Could not find location of shader uniform [vec3] named \"%\"", name);
			return;
		}
		glUniform3f(location, value.x, value.y, value.z);
	}

	void Shader::SetVector4(const char* name, glm::vec4 value) const noexcept
	{
		if (!_initialised) return;
		GLint location = glGetUniformLocation(_shaderProgramId, name);
		if (location == -1)
		{
			MIMIC_LOG_WARNING("[MimicRender::Shader] Could not find location of shader uniform [vec4] named \"%\"", name);
			return;
		}
		glUniform4fv(location, 1, glm::value_ptr(value));
	}

	void Shader::SetMat3(const char* name, const glm::mat3 value) const noexcept
	{
		if (!_initialised) return;
		GLint location = glGetUniformLocation(_shaderProgramId, name);
		if (location == -1)
		{
			MIMIC_LOG_WARNING("[MimicRender::Shader] Could not find location of shader uniform [matrix 3x3] named \"%\"", name);
			return;
		}
		glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(value));
	}

	void Shader::SetMat4(const char* name, const glm::mat4 value) const noexcept
	{
		if (!_initialised) return;
		GLint location = glGetUniformLocation(_shaderProgramId, name);
		if (location == -1)
		{
			MIMIC_LOG_WARNING("[MimicRender::Shader] Could not find location of shader uniform [matrix 4x4] named \"%\"", name);
			return;
		}
		glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(value));
	}

	void Shader::SetModelMatrix(const glm::mat4& value) noexcept
	{
		if (!_initialised) return;
		if (_modelMatrixUniformLocation == -1) return;
		glUniformMatrix4fv(_modelMatrixUniformLocation, 1, GL_FALSE, glm::value_ptr(value));
	}

	void Shader::SetViewMatrix(const glm::mat4& value) noexcept
	{
		if (!_initialised) return;
		if (_viewMatrixUniformLocation == -1) return;
		glUniformMatrix4fv(_viewMatrixUniformLocation, 1, GL_FALSE, glm::value_ptr(value));
	}

	void Shader::SetProjectionMatrix(const glm::mat4& value) noexcept
	{
		if (!_initialised) return;
		if (_projectionMatrixUniformLocation == -1) return;
		glUniformMatrix4fv(_projectionMatrixUniformLocation, 1, GL_FALSE, glm::value_ptr(value));
	}
	
	void Shader::SetCameraPosition(const glm::vec3& value) noexcept
	{
		if (!_initialised) return;
		if (_cameraPositionUniformLocation == -1) return;
		glUniform3f(_cameraPositionUniformLocation, value.x, value.y, value.z);
	}

	unsigned int Shader::GetSubroutineUniform(const GLenum& shaderType, const std::string& subroutineName)
	{
		if (!_initialised) return 0;
		const unsigned int subroutineLocation = glGetSubroutineUniformLocation(_shaderProgramId, shaderType, subroutineName.c_str());
		if (subroutineLocation == -1) MIMIC_LOG_WARNING("[MimicRender::Shader] Uniform subroutine location cannot be found.");
		return subroutineLocation;
	}

	unsigned int Shader::GetSubroutineIndex(const GLenum& shaderType, const std::string& subroutineFuncName)
	{
		if (!_initialised) return 0;
		const unsigned int subroutineIndex = glGetSubroutineIndex(_shaderProgramId, shaderType, subroutineFuncName.c_str());
		if (subroutineIndex == -1) MIMIC_LOG_WARNING("[MimicRender::Shader] Subroutine index location cannot be found.");
		return subroutineIndex;
	}
}