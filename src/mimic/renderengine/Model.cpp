#include "Model.h"
#include <utility/Logger.h>
#include <renderengine/Vertex.h>
#include <renderengine/Texture.h>
#include <renderengine/Mesh.h>
#include <glm/gtc/matrix_transform.hpp> 
#include <glm/gtc/type_ptr.hpp> 
// #include <lowlevelsystems/ResourceManager.h>
// #include <iostream>
#include <renderengine/Renderer.h>

// Source: https://learnopengl.com/Model-Loading/Model

namespace MimicRender
{
	const std::shared_ptr<Model> Model::Initialise()
	{
		return std::make_shared<Model>();
	}

	const std::shared_ptr<Model> Model::Initialise(const texture_vector& textures)
	{
		std::shared_ptr<Model> model = std::make_shared<Model>();
		model->_textures = textures;
		return model;
	}

	const std::shared_ptr<Model> Model::Initialise(const mesh_vector& meshes)
	{
		std::shared_ptr<Model> model = std::make_shared<Model>();
		model->_meshes = meshes;
		return model;
	}

	const std::shared_ptr<Model> Model::Initialise(const texture_vector& textures, const mesh_vector& meshes)
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

	void Model::QueMeshesToDraw(const std::shared_ptr<Shader>& shader, std::function<void()> onDrawLambda, std::shared_ptr<Renderer>& renderer)
	{
		for (auto mesh : _meshes)
		{
			renderer->AddToDrawQue(RenderObject::Initialise(
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

	//const int Model::Load(const std::string& path)
	//{
	//	Assimp::Importer importer;
	//	const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_CalcTangentSpace);

	//	if (!scene)
	//	{
	//		MIMIC_LOG_WARNING("[Model] Assimp importer encountered an error: %", importer.GetErrorString());
	//		return -1;
	//	}
	//	if (scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE)
	//	{
	//		MIMIC_LOG_WARNING("[Model] Assimp importer encountered an error: %", importer.GetErrorString());
	//		return -1;
	//	}
	//	if (!scene->mRootNode)
	//	{
	//		MIMIC_LOG_WARNING("[Model] Assimp importer encountered an error: %", importer.GetErrorString());
	//		return -1;
	//	}
	//	ProcessNode(scene->mRootNode, scene);
	//	return 0;
	//}

	const int Model::LoadMeshesFromFile(const std::string& path)
	{
		Assimp::Importer importer;
		const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_CalcTangentSpace);

		if (!scene)
		{
			MIMIC_LOG_WARNING("[Model] Assimp importer encountered an error: %", importer.GetErrorString());
			return -1;
		}
		if (scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE)
		{
			MIMIC_LOG_WARNING("[Model] Assimp importer encountered an error: %", importer.GetErrorString());
			return -1;
		}
		if (!scene->mRootNode)
		{
			MIMIC_LOG_WARNING("[Model] Assimp importer encountered an error: %", importer.GetErrorString());
			return -1;
		}
		ProcessNode(scene->mRootNode, scene);
		return 0;
	}

	void Model::ProcessNode(aiNode* node, const aiScene* scene)
	{
		// process all node's meshes:
		for (unsigned int i = 0; i < node->mNumMeshes; i++)
		{
			aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
			_meshes.push_back(ProcessMesh(mesh, scene));
		}

		// do the same for each of its children:
		for (unsigned int i = 0; i < node->mNumChildren; i++) ProcessNode(node->mChildren[i], scene);
	}

	const std::shared_ptr<Mesh> Model::ProcessMesh(aiMesh* mesh, const aiScene* scene)
	{
		// convert aiMesh into Mimic::Mesh: (vertex, normal, tex coords):
		vertex_vector vertices;
		std::vector<unsigned int> indices;
		const bool tangents = mesh->mTangents > 0;

		// get mesh vertices:
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

		// get mesh indices:
		for (unsigned int i = 0; i < mesh->mNumFaces; i++)
		{
			aiFace face = mesh->mFaces[i];
			const unsigned int faceIndicesLength = face.mNumIndices;
			for (unsigned int i = 0; i < faceIndicesLength; i++) indices.push_back(face.mIndices[i]);
		}

		std::shared_ptr<Mesh> newMesh = Mesh::Initialise(vertices, indices);
		return newMesh;

		//// mesh contains material index - retrieve material from scene:
		//if (mesh->mMaterialIndex >= 0)
		//{
		//	const aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];

		//	LoadMaterialTextures(material, aiTextureType_DIFFUSE, MIMIC_DIFFUSE | MIMIC_ALBEDO);
		//	LoadMaterialTextures(material, aiTextureType_SPECULAR, MIMIC_SPECULAR | MIMIC_ROUGHNESS);
		//	LoadMaterialTextures(material, aiTextureType_HEIGHT, MIMIC_NORMAL);
		//	LoadMaterialTextures(material, aiTextureType_AMBIENT, MIMIC_HEIGHT);
		//}
	}

	//const void Model::LoadMaterialTextures(const aiMaterial* material, const aiTextureType& type, const int& typeName)
	//{
	//	std::shared_ptr<Texture> loadedTexture;
	//	
	//	aiString aiPath;
	//	material->GetTexture(type, 0, &aiPath);
	//	std::string texturePath = aiPath.C_Str();
	//	if (texturePath == "") return;
	//	loadedTexture = GetResourceManager()->LoadResource<Texture>(texturePath);
	//	if (loadedTexture != nullptr)
	//	{
	//		loadedTexture->_type = typeName;
	//		loadedTexture->Name = texturePath;
	//		// check for repeats:
	//		for (auto texture : _materialTextures)
	//		{
	//			if (loadedTexture->_type == texture->_type) return;
	//		}
	//		_materialTextures.push_back(loadedTexture);
	//	}
	//}
}