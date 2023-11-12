#pragma once
#include <string>
#include <memory>
#include <lowlevelsystems/Resource.h>
#include <unordered_map>
#include <GL/glew.h>
#include <GLM/glm.hpp>

// Source: https://learnopengl.com/Getting-started/Shaders

namespace Mimic
{
	// #############################################################################
	// shader stuct:
	// #############################################################################
	struct Shader : Resource
	{
		const int Load(const std::string& path) override;

	private:
		friend struct Renderer;
		friend struct Material;
		friend struct CubeMap;
		friend struct BasicMaterial;
		friend struct PBRMaterial;

		const std::string ReadShaderFile(const std::string& path);
		static GLenum ShaderTypeFromString(const std::string& type);
		const std::unordered_map<GLuint, std::string> PreProcess(const std::string& source);
		void CompileShaderText(const std::unordered_map<GLenum, std::string>& shaderSources);

		void SetModelMatrix(const glm::mat4& value) noexcept;
		void SetViewMatrix(const glm::mat4& value) noexcept;
		void SetProjectionMatrix(const glm::mat4& value) noexcept;
		void SetTexture(const char* name, const int& textureId, const int& bindPoint);
		void SetBool(const char* name, const bool value) const noexcept;
		void SetInt(const char* name, const int value) const noexcept;
		void SetFloat(const char* name, const float value) const noexcept;
		void SetVector3(const char* name, const glm::vec3 value) const noexcept;
		void SetVector4(const char* name, glm::vec4 value) const noexcept;
		void SetMat4(const char* name, const glm::mat4 value) const noexcept;

		int _modelMatrixUniformLocation = -1;
		int _viewMatrixUniformLocation = -1;
		int _projectionMatrixUniformLocation = -1;
		unsigned int _shaderProgramId;
		bool _initialised;
	};
}