#pragma once
#include <GL/glew.h>
#include <string>
#include <assimp/Importer.hpp>      
#include <assimp/scene.h>           
#include <assimp/postprocess.h>    
#include <lowlevelsystems/Component.h>
#include "Mesh.h"
#include "Shader.h"

namespace Mimic
{
	enum RendererType
	{
		Triangle
	};
	
	// #############################################################################
	// renderer stuct:
	// #############################################################################

	struct Renderer : Component
	{
		RendererType Type;
		int VertexCount;
		GLuint PositionsVertexBufferId;
		GLuint VertexArrayId;
		GLuint ShaderProgramId;
		GLuint VertexShaderId;
		GLuint FragmentShaderId;
		std::shared_ptr<int> Number;

		void Initialise(RendererType type);
		void Render();
		/*const aiScene* AssimpModelImport(const std::string& pFile);*/

		private:
		std::shared_ptr<Shader> _shader;
		std::shared_ptr<Mesh> _mesh;
	};
}