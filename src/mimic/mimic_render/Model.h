#pragma once
#include <functional>
#include <GLM/glm.hpp>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

namespace MimicRender
{
	struct Texture;
	struct Mesh;
	struct Shader;
	struct Renderer;

	typedef std::vector<std::shared_ptr<Texture>> texture_vector;
	typedef std::vector<std::shared_ptr<Mesh>> mesh_vector;

	/// <summary>
	/// Holds all data relating to a model (including the model's textures and meshes).
	/// </summary>
	struct Model
	{
		static std::shared_ptr<Model> Initialise();
		static std::shared_ptr<Model> Initialise(const std::string& path);
		static std::shared_ptr<Model> Initialise(const texture_vector& textures);
		static std::shared_ptr<Model> Initialise(const mesh_vector& meshes);
		static std::shared_ptr<Model> Initialise(const texture_vector& textures, const mesh_vector& meshes);

		/// <summary> Push back mesh to meshes list. </summary>
		void AddMesh(const std::shared_ptr<Mesh>& mesh);

		/// <summary> Push back texture to textures list. </summary>
		void AddTexture(const std::shared_ptr<Texture>& texture);

		/// <summary>
		/// Update model matrix based on the model's position (translate), rotation (rotate), and scale.
		/// </summary>
		/// <param name="position">Current position of the model.</param>
		/// <param name="rotation">Current rotation (eular angles) of the model.</param>
		/// <param name="scale">Current scale of the model.</param>
		void UpdateModelMatrix(const glm::vec3& position, const glm::vec3& rotation, const glm::vec3& scale);

		/// <summary>
		/// Converts all meshes into render objects and adds them to the inputted renderer's
		/// render queue. These render objects will stay there until Renderer::Draw() is called.
		/// </summary>
		/// <param name="shader">Shader code that will be executed by the GPU to render the model's meshes.</param>
		/// <param name="onDrawLambda">Draw lambda primarily used to set shader uniform values.</param>
		/// <param name="renderer">Renderer that will queue the model's meshes to draw (see Renderer::Draw()). </param>
		void QueueMeshesToDraw(const std::shared_ptr<Shader>& shader, std::function<void()> onDrawLambda, std::shared_ptr<Renderer>& renderer);

		std::vector<std::shared_ptr<Mesh>> GetMeshes() const;

	private:
		glm::mat4 _modelMatrix = glm::mat4(1.0f);
		glm::mat4 _normalMatrix = glm::mat3(1.0f);
		texture_vector _textures;
		mesh_vector _meshes;

		const int LoadMeshesFromFile(const std::string& path);
		void ProcessNode(aiNode* node, const aiScene* scene);
		std::shared_ptr<Mesh> ProcessMesh(aiMesh* mesh, const aiScene* scene) const;
	};
}