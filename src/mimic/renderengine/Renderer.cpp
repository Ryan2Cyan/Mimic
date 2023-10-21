#include "Renderer.h"
#include <fstream>
#include <iostream>
#include <vector>

namespace Mimic
{
	void Renderer::Initialise(const RendererType type)
	{
		Type = type;

		std::vector<Vertex> positions;
		std::vector<unsigned int> indices;
		std::vector<Texture> textures;

		switch (Type)
		{
			case Mimic::Triangle:
			{
				positions = {
					Vertex(glm::vec3(-0.5f, -0.5f, 0.0f), glm::vec3(0.0f), glm::vec2(0.0f)),
					Vertex(glm::vec3(0.5f, -0.5f, 0.0f), glm::vec3(0.0f), glm::vec2(0.0f)),
					Vertex(glm::vec3(0.0f,  0.5f, 0.0f), glm::vec3(0.0f), glm::vec2(0.0f))
				};
				indices = { 0, 1, 2 };
				textures = { };
			} break;

			default: break;
		}

		_mesh = std::make_shared<Mesh>(positions, indices, textures);

		_shader = std::make_shared<Shader>(
			"../src/mimic/shaders/VertexShader.txt",
			"../src/mimic/shaders/FragmentShader.txt");
	}

	//// Source: https://assimp-docs.readthedocs.io/en/latest/usage/use_the_lib.html
	//const aiScene* Renderer::AssimpModelImport(const std::string& pFile)
	//{
	//	Assimp::Importer importer;

	//	const aiScene* scene = importer.ReadFile(pFile,
	//	  aiProcess_CalcTangentSpace |
	//	  aiProcess_Triangulate |
	//	  aiProcess_JoinIdenticalVertices |
	//	  aiProcess_SortByPType);

	//	if (nullptr == scene)
	//	{
	//		std::cerr << "WARNING: Failed to load model (assimpmodelloader): " << importer.GetErrorString() << std::endl;
	//		return nullptr;
	//	}
	//	return scene;
	//}

	void Renderer::Render()
	{
		_shader->Activate();
		_mesh->Draw(_shader);
	}
}