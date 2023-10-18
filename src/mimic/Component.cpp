#include "Component.h"
#include <fstream>
#include <iostream>


namespace Mimic
{
	// component: ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	std::shared_ptr<Component> Component::Initialize() noexcept { return std::make_shared<Component>(); }


	// triangle renderer: ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	TriangleRenderer::TriangleRenderer()
	{
		Initialise();
	}

	TriangleRenderer::~TriangleRenderer()
	{
		glDeleteShader(VertexShaderId);
		glDeleteShader(FragmentShaderId);
	}

	const char* TriangleRenderer::ReadShaderFile(const char* const fileName)
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

	void TriangleRenderer::CompileShaderText(const char* const fileText, const GLenum shaderType, GLuint& shaderId)
	{
		shaderId = glCreateShader(shaderType);
		glShaderSource(shaderId, 1, &fileText, NULL);
		glCompileShader(shaderId);
		int success;
		char infoLog[512];
		glGetShaderiv(shaderId, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			glGetShaderInfoLog(shaderId, 512, NULL, infoLog);
			std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
		}
	}

	void TriangleRenderer::Initialise()
	{
		float triangleVertices[] = {
		-0.5f, -0.5f, 0.0f,
		0.5f, -0.5f, 0.0f,
		0.0f,  0.5f, 0.0f };
		float data_size = sizeof(float) * VertexCount * 3;
		VertexCount = 3;

		// generate vertex array object:
		glGenVertexArrays(1, &VertexArrayId);
		glBindVertexArray(VertexArrayId);

		// generate vertex buffer object:
		glGenBuffers(1, &PositionsVertexBufferId);
		glBindBuffer(GL_ARRAY_BUFFER, PositionsVertexBufferId);
		glBufferData(GL_ARRAY_BUFFER, data_size, triangleVertices, GL_STATIC_DRAW);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);

		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);
		glDisableVertexAttribArray(0);

		// read shaders:
		const char* vertexShaderText = ReadShaderFile("VertexShader.txt");
		const char* fragmentShaderText = ReadShaderFile("FragmentShader.txt");

		// compile shaders:
		CompileShaderText(vertexShaderText, GL_VERTEX_SHADER, VertexShaderId);
		CompileShaderText(fragmentShaderText, GL_FRAGMENT_SHADER, FragmentShaderId);

		// link shaders to program:
		ShaderProgramId = glCreateProgram();
		glAttachShader(ShaderProgramId, VertexShaderId);
		glAttachShader(ShaderProgramId, FragmentShaderId);
		glLinkProgram(ShaderProgramId);
		int success;
		char infoLog[512];
		glGetProgramiv(ShaderProgramId, GL_LINK_STATUS, &success);
		if (!success)
		{
			glGetProgramInfoLog(ShaderProgramId, 512, NULL, infoLog);
			std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
		}
	}

	void TriangleRenderer::Render()
	{
		glUseProgram(ShaderProgramId);
		glBindVertexArray(VertexArrayId);
		glDrawArrays(GL_TRIANGLES, 0, VertexCount);
		glBindVertexArray(0);
	}

	void TriangleRenderer::Display()
	{

	}
}