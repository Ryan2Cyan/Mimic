#include <GL/glew.h>
#include <assimp/Importer.hpp>      
#include <assimp/scene.h>           
#include <assimp/postprocess.h>    
#include "Component.h"

namespace Mimic
{
	enum RendererType
	{
		Triangle
	};
	

	struct Renderer : Component
	{
		RendererType Type;
		int VertexCount;
		GLuint PositionsVertexBufferId;
		GLuint VertexArrayId;
		GLuint ShaderProgramId;
		GLuint VertexShaderId;
		GLuint FragmentShaderId;
		GLuint ProjectionLocation;
		GLuint ViewLocation;
		GLuint ModelLocation;

		void Initialise();
		void Render();
		void Display();
		const aiScene* AssimpModelImport(const std::string& pFile);

	private:
		const char* ReadShaderFile(const char* const fileName);
		void CompileShaderText(const char* fileText, const GLenum shaderType, GLuint& shaderId);
	};
}