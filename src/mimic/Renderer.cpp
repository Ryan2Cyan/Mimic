#include "Renderer.h"
#include <fstream>
#include <iostream>
#include <vector>

namespace Mimic
{
	// Source: https://assimp-docs.readthedocs.io/en/latest/usage/use_the_lib.html
	const aiScene* Renderer::AssimpModelImport(const std::string& pFile)
	{
		Assimp::Importer importer;

		const aiScene* scene = importer.ReadFile(pFile,
		  aiProcess_CalcTangentSpace |
		  aiProcess_Triangulate |
		  aiProcess_JoinIdenticalVertices |
		  aiProcess_SortByPType);

		if (nullptr == scene)
		{
			std::cerr << "WARNING: Failed to load model (assimpmodelloader): " << importer.GetErrorString() << std::endl;
			return nullptr;
		}
		return scene;
	}

	const char* Renderer::ReadShaderFile(const char* const fileName)
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

	void Renderer::CompileShaderText(const char* const fileText, const GLenum shaderType, GLuint& shaderId)
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

	void Renderer::Initialise()
	{
		// decide vertex data:
		std::vector<float> vertexPositions;
		switch (Type)
		{
			case Triangle:
			{
				VertexCount = 3;
				vertexPositions = {
					-0.5f, -0.5f, 0.0f,
					 0.5f, -0.5f, 0.0f,
					 0.0f,  0.5f, 0.0f 
				};
			}

			default: break;
		}
		const float dataSize = sizeof(float) * VertexCount * 3;

		// generate vertex array object:
		glGenVertexArrays(1, &VertexArrayId);
		glBindVertexArray(VertexArrayId);

		// generate vertex buffer object:
		glGenBuffers(1, &PositionsVertexBufferId);
		glBindBuffer(GL_ARRAY_BUFFER, PositionsVertexBufferId);
		glBufferData(GL_ARRAY_BUFFER, dataSize, &vertexPositions[0], GL_STATIC_DRAW);
		glVertexAttribPointer(0, VertexCount, GL_FLOAT, GL_FALSE, VertexCount * sizeof(float), (void*)0);
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

	void Renderer::Render()
	{
		glUseProgram(ShaderProgramId);
		glBindVertexArray(VertexArrayId);
		glDrawArrays(GL_TRIANGLES, 0, VertexCount);
		glBindVertexArray(0);
	}

	void Renderer::Display()
	{

	}
}