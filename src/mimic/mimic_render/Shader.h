#pragma once
#include <mimic_render/Texture.h>
#include <unordered_map>

// Source: https://learnopengl.com/Getting-started/Shaders

namespace MimicRender
{
	/// <summary>
	/// Responsible for executing GPU code to render scene objects. See the Material and 
	/// ModelRenderer structs.
	/// </summary>
	struct Shader
	{
		static std::shared_ptr<Shader> Initialise(const std::string& path);

		void SetModelMatrix(const glm::mat4& value) noexcept;
		void SetViewMatrix(const glm::mat4& value) noexcept;
		void SetProjectionMatrix(const glm::mat4& value) noexcept;
		void SetCameraPosition(const glm::vec3& value) noexcept;

		void SetTexture(const char* name, const int& textureId, const int& bindPoint, const std::uint32_t& textureParams = MimicRender::Texture::MIMIC_2D_TEXTURE);
		void SetBool(const char* name, const bool value) const noexcept;
		void SetInt(const char* name, const int value) const noexcept;
		void SetFloat(const char* name, const float value) const noexcept;
		void SetVector3(const char* name, const glm::vec3 value) const noexcept;
		void SetVector4(const char* name, glm::vec4 value) const noexcept;
		void SetMat3(const char* name, const glm::mat3 value) const noexcept;
		void SetMat4(const char* name, const glm::mat4 value) const noexcept;
		unsigned int GetSubroutineUniform(const GLenum& shaderType, const std::string& subroutineName);
		unsigned int GetSubroutineIndex(const GLenum& shaderType, const std::string& subroutineFuncName);

		unsigned int _shaderProgramId;
	private:
		friend struct Renderer;
		friend struct EnvironmentCubeMap;
		
		void UseShader() const noexcept;
		static std::string ReadShaderFile(const std::string& path);
		static GLenum ShaderTypeFromString(const std::string& type);
		static std::unordered_map<GLuint, std::string> PreProcess(const std::string& source);
		static std::shared_ptr<Shader> CompileShaderText(const std::unordered_map<GLenum, std::string>& shaderSources);

		int _modelMatrixUniformLocation = -1;
		int _viewMatrixUniformLocation = -1;
		int _projectionMatrixUniformLocation = -1;
		int _cameraPositionUniformLocation = -1;
	
		bool _initialised;
	};
}