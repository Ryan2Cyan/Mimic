#pragma once

// #include <lowlevelsystems/Resource.h>
// #include <lowlevelsystems/Component.h>
#include <renderengine/Mesh.h>
#include <memory>
#include <vector>
#include <functional>
#include <GLM/glm.hpp>
// #include <string>
// #include <assimp/Importer.hpp>
// #include <assimp/scene.h>
// #include <assimp/postprocess.h>

namespace MimicRender
{
	struct Texture;
	struct Mesh;
	struct Shader;
	struct Renderer;

	typedef std::vector<std::shared_ptr<Texture>> texture_vector;
	typedef std::vector<std::shared_ptr<Mesh>> mesh_vector;

	// #############################################################################
	// model stuct:
	// #############################################################################
	// struct Shader;

	struct Model
	{
		static const std::shared_ptr<Model> Initialise();
		static const std::shared_ptr<Model> Initialise(const texture_vector& textures);
		static const std::shared_ptr<Model> Initialise(const mesh_vector& meshes);
		static const std::shared_ptr<Model> Initialise(const texture_vector& textures, const mesh_vector& meshes);

		void AddMesh(const std::shared_ptr<Mesh>& mesh);
		void AddTexture(const std::shared_ptr<Texture>& texture);
		void UpdateModelMatrix(const glm::vec3& position, const glm::vec3& rotation, const glm::vec3& scale);
		void QueMeshesToDraw(const std::shared_ptr<Shader>& shader, std::function<void()> onDrawLambda, std::shared_ptr<Renderer>& renderer);

	private:
		glm::mat4 _modelMatrix = glm::mat4(1.0f);
		texture_vector _textures;
		mesh_vector _meshes;
		/*friend struct ResourceManager;
		friend struct ModelRenderer;
		friend struct CubeMap;
		friend struct EnvironmentCubeMap;*/

		//// const int Load(const std::string& path) override;
		//const int Create(const std::string& path);
		//void ProcessNode(aiNode* node, const aiScene* scene);
		//const std::shared_ptr<Mesh> ProcessMesh(aiMesh* mesh, const aiScene* scene);
		//const void LoadMaterialTextures(const aiMaterial* material, const aiTextureType& type, const int& typeName);
	};
}