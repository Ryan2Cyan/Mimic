#include "Model.h"
#include <mimic_utility/Logger.h>
#include <mimic_render/Vertex.h>
#include <mimic_render/Texture.h>
#include <mimic_render/Renderer.h>
#include <mimic_render/Mesh.h>
#include <glm/gtc/matrix_transform.hpp> 
#include <glm/gtc/type_ptr.hpp> 

// Source: https://learnopengl.com/Model-Loading/Model

namespace MimicRender
{
	std::shared_ptr<Model> Model::Initialise()
	{
		return std::make_shared<Model>();
	}

	std::shared_ptr<Model> Model::Initialise(const std::string& path)
	{
		std::shared_ptr<Model> model = std::make_shared<Model>();
		model->LoadMeshesFromFile(path);
		return model;
	}

	std::shared_ptr<Model> Model::Initialise(const texture_vector& textures)
	{
		std::shared_ptr<Model> model = std::make_shared<Model>();
		model->_textures = textures;
		return model;
	}

	std::shared_ptr<Model> Model::Initialise(const mesh_vector& meshes)
	{
		std::shared_ptr<Model> model = std::make_shared<Model>();
		model->_meshes = meshes;
		return model;
	}

	std::shared_ptr<Model> Model::Initialise(const texture_vector& textures, const mesh_vector& meshes)
	{
		std::shared_ptr<Model> model = std::make_shared<Model>();
		model->_textures = textures;
		model->_meshes = meshes;
		return model;
	}

	void Model::UpdateModelMatrix(const glm::vec3& position, const glm::vec3& rotation, const glm::vec3& scale)
	{
		_modelMatrix = glm::translate(glm::mat4(1.0f), position) * glm::mat4_cast(glm::quat(rotation)) * glm::scale(glm::mat4(1.0f), scale);
	}

	void Model::QueueMeshesToDraw(const std::shared_ptr<Shader>& shader, std::function<void()> onDrawLambda, std::shared_ptr<Renderer>& renderer)
	{
		for (auto mesh : _meshes)
		{
			renderer->AddToDrawQueue(RenderObject::Initialise(
				mesh->_vertexArrayId,
				mesh->_dataSize,
				shader,
				_modelMatrix,
				onDrawLambda
			));
		}
	}

	void Model::AddMesh(const std::shared_ptr<Mesh>& mesh)
	{
		if (mesh == nullptr)
		{
			MIMIC_LOG_WARNING("[MimicRender::Model] Unable to add uninitialised mesh.");
			return;
		}
		_meshes.push_back(mesh);
	}

	void Model::AddTexture(const std::shared_ptr<Texture>& texture)
	{
		if (texture == nullptr)
		{
			MIMIC_LOG_WARNING("[MimicRender::Model] Unable to add uninitialised texture.");
			return;
		}
		_textures.push_back(texture);
	}

	const int Model::LoadMeshesFromFile(const std::string& path)
	{
		Assimp::Importer importer;
		const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_CalcTangentSpace);

		if (!scene)
		{
			MIMIC_LOG_WARNING("[MimicRender::Model] Assimp importer encountered an error: %", importer.GetErrorString());
			return -1;
		}
		if (scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE)
		{
			MIMIC_LOG_WARNING("[MimicRender::Model] Assimp importer encountered an error: %", importer.GetErrorString());
			return -1;
		}
		if (!scene->mRootNode)
		{
			MIMIC_LOG_WARNING("[MimicRender::Model] Assimp importer encountered an error: %", importer.GetErrorString());
			return -1;
		}
		ProcessNode(scene->mRootNode, scene);
		return 0;
	}

	void Model::ProcessNode(aiNode* node, const aiScene* scene)
	{
		// Process all node's meshes:
		for (unsigned int i = 0; i < node->mNumMeshes; i++)
		{
			aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
			_meshes.push_back(ProcessMesh(mesh, scene));
		}

		// Do the same for each of its children:
		for (unsigned int i = 0; i < node->mNumChildren; i++) ProcessNode(node->mChildren[i], scene);
	}

	const std::shared_ptr<Mesh> Model::ProcessMesh(aiMesh* mesh, const aiScene* scene) const
	{
		// Convert aiMesh into Mimic::Mesh: (verties, normals, and texture coordinates):
		vertex_vector vertices;
		std::vector<unsigned int> indices;
		const bool tangents = mesh->mTangents > 0;

		// Get mesh vertices:
		for (unsigned int i = 0; i < mesh->mNumVertices; i++)
		{
			const aiVector3D meshVertices = mesh->mVertices[i];
			const aiVector3D meshNormals = mesh->mNormals[i];
			aiVector3D meshTangents;
			aiVector3D meshBitangents;
			if (tangents)
			{
				meshTangents = mesh->mTangents[i];
				meshBitangents = mesh->mBitangents[i];
			}

			Vertex vertex = Vertex::Initialise(
				glm::vec3(meshVertices.x, meshVertices.y, meshVertices.z),
				glm::vec3(meshNormals.x, meshNormals.y, meshNormals.z),
				glm::vec3(meshTangents.x, meshTangents.y, meshTangents.z),
				glm::vec3(meshBitangents.x, meshBitangents.y, meshBitangents.z),
				glm::vec2(0.0f)
			);

			if (mesh->mTextureCoords[0]) vertex._textureCoordinates = glm::vec2(mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y);

			vertices.push_back(vertex);
		}

		// Get mesh indices:
		for (unsigned int i = 0; i < mesh->mNumFaces; i++)
		{
			aiFace face = mesh->mFaces[i];
			const unsigned int faceIndicesLength = face.mNumIndices;
			for (unsigned int i = 0; i < faceIndicesLength; i++) indices.push_back(face.mIndices[i]);
		}

		std::shared_ptr<Mesh> newMesh = Mesh::Initialise(vertices, indices);
		return newMesh;
	}
}