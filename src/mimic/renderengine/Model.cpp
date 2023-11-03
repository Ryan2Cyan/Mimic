#include "Model.h"
#include <utility/Logger.h>
#include <renderengine/Vertex.h>
#include <renderengine/Texture.h>
#include <lowlevelsystems/ResourceManager.h>
#include <iostream>

// Source: https://learnopengl.com/Model-Loading/Model

namespace Mimic
{
	const int Model::Load(const std::string& path)
	{
		Assimp::Importer importer;
		const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);

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
		std::vector<Vertex> vertices;
		std::vector<unsigned int> indices;
		std::vector<std::shared_ptr<Texture>> textures;

		for (unsigned int i = 0; i < mesh->mNumVertices; i++)
		{
			const aiVector3D meshVertices = mesh->mVertices[i];
			const aiVector3D meshNormals = mesh->mNormals[i];

			Vertex newVertex(
				glm::vec3(meshVertices.x, meshVertices.y, meshVertices.z),
				glm::vec3(meshNormals.x, meshNormals.y, meshNormals.z),
				glm::vec2(0.0f)
			);

			if (mesh->mTextureCoords[0]) newVertex._textureCoordinates = glm::vec2(mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y);

			vertices.push_back(newVertex);
		}

		// store face indices (each face representing a primitive/triangle):
		for (unsigned int i = 0; i < mesh->mNumFaces; i++)
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
		if (textures.size() < 1) MIMIC_LOG_WARNING("[Model] Mesh \"%\" loaded with no textures.", Name.c_str());
		return newMesh;
	}

	const std::vector<std::shared_ptr<Texture>> Model::LoadMaterialTextures(const aiMaterial* material, const aiTextureType& type, const std::string& typeName)
	{
		std::vector<std::shared_ptr<Texture>> loadedTextures;

		const unsigned int length = material->GetTextureCount(type);
		for (unsigned int i = 0; i < length; i++)
		{
			aiString aiPath;
			material->GetTexture(type, i, &aiPath);
			std::string texturePath = aiPath.C_Str();
			std::shared_ptr<Texture> loadedTexture = GetResourceManager()->LoadResource<Texture>(texturePath);
			if (loadedTexture != nullptr)
			{
				/*loadedTexture->Type = type;*/
				loadedTextures.push_back(loadedTexture);
			}
			else MIMIC_LOG_WARNING("[Model] Unable to load texture of type: \"%\", from path: \"%\"", typeName, texturePath);
		}
		return loadedTextures;
	}
}