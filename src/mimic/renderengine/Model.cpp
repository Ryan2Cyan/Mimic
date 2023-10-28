#include "Model.h"
#include <renderengine/Vertex.h>
#include <renderengine/Texture.h>
#include <lowlevelsystems/ResourceManager.h>
#include <iostream>

// Source: https://learnopengl.com/Model-Loading/Model

namespace Mimic
{
	Model::Model() {}

	void Model::Draw(std::shared_ptr<Shader> shader)
	{
		const unsigned int length = _meshes.size();
		for (unsigned int i = 0; i < length; i++) _meshes[i]->Draw(shader);
	}

	std::shared_ptr<Component> Model::GetComponent() const
	{
		return Component.lock();
	}

	void Model::Load(const std::string& path)
	{
		Assimp::Importer importer;
		const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);

		if (!scene)
		{
			std::cout << "ERROR::ASSIMP::" << importer.GetErrorString() << std::endl;
			return;
		}
		if (scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE)
		{
			std::cout << "ERROR::ASSIMP::" << importer.GetErrorString() << std::endl;
			return;
		}
		if (!scene->mRootNode)
		{
			std::cout << "ERROR::ASSIMP::" << importer.GetErrorString() << std::endl;
			return;
		}

		_directory = path.substr(0, path.find_last_of('/'));
		ProcessNode(scene->mRootNode, scene);
	}

	void Model::ProcessNode(aiNode* node, const aiScene* scene)
	{
		// process all node's meshes:
		unsigned int length = node->mNumMeshes;
		for (unsigned int i = 0; i < length; i++)
		{
			aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
			_meshes.push_back(ProcessMesh(mesh, scene));
		}

		// do the same for each of its children:
		length = node->mNumChildren;
		for (unsigned int i = 0; i < length; i++) ProcessNode(node->mChildren[i], scene);
	}

	std::shared_ptr<Mesh> Model::ProcessMesh(aiMesh* mesh, const aiScene* scene)
	{
		// convert aiMesh into Mimic::Mesh: (vertex, normal, tex coords):
		std::vector<Vertex> vertices;
		std::vector<unsigned int> indices;
		std::vector<std::shared_ptr<Texture>> textures;

		unsigned int length = mesh->mNumVertices;
		for (unsigned int i = 0; i < length; i++)
		{
			const aiVector3D meshVertices = mesh->mVertices[i];
			const aiVector3D meshNormals = mesh->mNormals[i];

			Vertex newVertex(
				glm::vec3(meshVertices.x, meshVertices.y, meshVertices.z),
				glm::vec3(meshNormals.x, meshNormals.y, meshNormals.z),
				glm::vec2(0.0f)
			);

			if (mesh->mTextureCoords[0]) newVertex.TextureCoordinates = glm::vec2(mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y);

			vertices.push_back(newVertex);
		}

		// store face indices (each face representing a primitive/triangle):
		length = mesh->mNumFaces;
		for (unsigned int i = 0; i < length; i++)
		{
			aiFace face = mesh->mFaces[i];
			const unsigned int faceIndicesLength = face.mNumIndices;
			for (unsigned int i = 0; i < faceIndicesLength; i++) indices.push_back(face.mIndices[i]);
		}

		// mesh contains material index - retrieve material from scene:
		if (mesh->mMaterialIndex >= 0)
		{
			aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];

			const std::vector<std::shared_ptr<Texture>> diffuseMaps = LoadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse");
			textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());

			const std::vector<std::shared_ptr<Texture>> specularMaps = LoadMaterialTextures(material, aiTextureType_SPECULAR, "texture_specular");
			textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());

			const std::vector<std::shared_ptr<Texture>> normalMaps = LoadMaterialTextures(material, aiTextureType_HEIGHT, "texture_normal");
			textures.insert(textures.end(), normalMaps.begin(), normalMaps.end());

			const std::vector<std::shared_ptr<Texture>> heightMaps = LoadMaterialTextures(material, aiTextureType_AMBIENT, "texture_height");
			textures.insert(textures.end(), heightMaps.begin(), heightMaps.end());
		}

		std::shared_ptr<Mesh> newMesh = std::make_shared<Mesh>(vertices, indices, textures);
		if (textures.size() < 1) std::cout << "WARNING: Mesh on [" << GetComponent()->GetGameObject()->Name << "] has no textures." << std::endl;
		return newMesh;
	}

	const std::vector<std::shared_ptr<Texture>> Model::LoadMaterialTextures(aiMaterial* material, aiTextureType type, std::string typeName)
	{
		std::vector<std::shared_ptr<Texture>> loadedTextures;

		const unsigned int length = material->GetTextureCount(type);
		for (unsigned int i = 0; i < length; i++)
		{
			aiString aiPath;
			material->GetTexture(type, i, &aiPath);
			std::string stringPath = aiPath.C_Str();
			std::shared_ptr<Texture> loadedTexture = GetResourceManager()->LoadResource<Texture>(stringPath);
			loadedTexture->LoadTextureFromFile(aiPath.C_Str(), _directory, false);
			loadedTexture->Type = type;
			loadedTextures.push_back(loadedTexture);
		}
		return loadedTextures;
	}
}