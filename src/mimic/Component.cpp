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

	}

	void TriangleRenderer::Initialise()
	{
		// generate position data:
		float triangleVertices[] = {
		-0.5f, -0.5f, 0.0f,
		0.5f, -0.5f, 0.0f,
		0.0f,  0.5f, 0.0f };

		glGenBuffers(1, &PositionsVertexBufferId);
		glBindBuffer(GL_ARRAY_BUFFER, PositionsVertexBufferId);
		glBufferData(GL_ARRAY_BUFFER, sizeof(VertexCount), triangleVertices, GL_STATIC_DRAW);

		// load vertex shader:
		std::ifstream shaderFile("VertexShader");
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
				std::cerr << "WARNING: could not read shader from file: " << "VertexShader" << std::endl;
				return;
			}
			length = (int)shaderFile.gcount(); // Insert null character at last valid character in the array.
			shaderText[length - 1] = 0;
			shaderFile.close();
		}
		else std::cerr << "WARNING: could not open shader from file: " << "VertexShader" << std::endl;

		// compile vertex shader:
		GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(vertexShader, 1, &shaderText, NULL);
		glCompileShader(vertexShader);
		int success;
		char infoLog[512];
		glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
		if (!success) { glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl; }
	}

	void TriangleRenderer::Render()
	{

	}

	void TriangleRenderer::Display()
	{

	}
}