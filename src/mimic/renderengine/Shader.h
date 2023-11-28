#pragma once
#include <string>
#include <memory>
#include <unordered_map>
#include <GL/glew.h>
#include <GLM/glm.hpp>

// Source: https://learnopengl.com/Getting-started/Shaders

namespace MimicRender
{
	// #############################################################################
	// shader stuct:
	// #############################################################################
	struct Shader
	{
		static const std::shared_ptr<Shader> Initialise(const std::string& path);

		void SetModelMatrix(const glm::mat4& value) noexcept;
		void SetViewMatrix(const glm::mat4& value) noexcept;
		void SetProjectionMatrix(const glm::mat4& value) noexcept;
		void SetTexture(const char* name, const int& textureId, const int& bindPoint);
		void SetBool(const char* name, const bool value) const noexcept;
		void SetInt(const char* name, const int value) const noexcept;
		void SetFloat(const char* name, const float value) const noexcept;
		void SetVector3(const char* name, const glm::vec3 value) const noexcept;
		void SetVector4(const char* name, glm::vec4 value) const noexcept;
		void SetMat3(const char* name, const glm::mat3 value) const noexcept;
		void SetMat4(const char* name, const glm::mat4 value) const noexcept;
		const unsigned int GetSubroutineUniform(const GLenum& shaderType, const std::string& subroutineName);
		const unsigned int GetSubroutineIndex(const GLenum& shaderType, const std::string& subroutineFuncName);

		void UseShader() const noexcept; // move to private
	private:
		friend struct Renderer;
		friend struct EnvironmentCubeMap;
		

		static const std::string ReadShaderFile(const std::string& path);
		static GLenum ShaderTypeFromString(const std::string& type);
		static const std::unordered_map<GLuint, std::string> PreProcess(const std::string& source);
		static const std::shared_ptr<Shader> CompileShaderText(const std::unordered_map<GLenum, std::string>& shaderSources);

		int _modelMatrixUniformLocation = -1;
		int _viewMatrixUniformLocation = -1;
		int _projectionMatrixUniformLocation = -1;
		unsigned int _shaderProgramId;
		bool _initialised;
	};
}