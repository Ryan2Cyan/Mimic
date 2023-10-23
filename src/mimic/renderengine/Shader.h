#pragma once
#include <string>
#include <fstream>
#include <memory>
#include <GL/glew.h>
#include <GLM/glm.hpp>

// Source: https://learnopengl.com/Getting-started/Shaders

namespace Mimic
{
	// #############################################################################
	// shader stuct:
	// #############################################################################
	struct Shader
	{
		explicit Shader(const std::string vertexShaderPath, const std::string fragmentShaderPath);

		unsigned int ShaderProgramId;
		
		std::shared_ptr<Shader> Initialise(const std::string vertexShaderPath, const std::string fragmentShaderPath) const;
		void Activate();
		void SetBool(const char* name, const bool value) const;
		void SetInt(const char* name, const int value) const;
		void SetFloat(const char* name, const float value) const;
		void SetMat4(const char* name, const glm::mat4 value) const;

	private:
		friend struct ModelRenderer;

		const char* ReadShaderFile(const char* const fileName);
		const unsigned int CompileShaderText(const char* fileText, const unsigned int shaderType);
		void SetModelMatrix(const glm::mat4 value);
		void SetViewMatrix(const glm::mat4 value);
		void SetProjectionMatrix(const glm::mat4 value);

		int _modelMatrixUniformLocation;
		int _viewMatrixUniformLocation;
		int _projectionMatrixUniformLocation;
	};
}