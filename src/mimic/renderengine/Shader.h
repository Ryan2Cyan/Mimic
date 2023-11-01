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
		unsigned int ShaderProgramId;
		

		const int Load(const std::string& path) override;

		void SetBool(const char* name, const bool value) const;
		void SetInt(const char* name, const int value) const;
		void SetFloat(const char* name, const float value) const;
		void SetMat4(const char* name, const glm::mat4 value) const;

	private:
		friend struct ModelRenderer;

		const std::string ReadShaderFile(const std::string& path);
		static GLenum ShaderTypeFromString(const std::string& type);
		std::unordered_map<GLuint, std::string> PreProcess(const std::string& source);
		void CompileShaderText(const std::unordered_map<GLenum, std::string>& shaderSources);

		void SetModelMatrix(const glm::mat4 value);
		void SetViewMatrix(const glm::mat4 value);
		void SetProjectionMatrix(const glm::mat4 value);

		int _modelMatrixUniformLocation;
		int _viewMatrixUniformLocation;
		int _projectionMatrixUniformLocation;

		bool _initialised;
	};
}