#pragma once
#include <GL/glew.h>
#include <memory>
#include <vector>

#include "Entity.h"

namespace Mimic
{
	struct Component
	{
		std::shared_ptr<Component> Initialize() noexcept;

		std::weak_ptr<Entity> Entity;
	};

	struct TriangleRenderer : Component
	{
		TriangleRenderer();
		~TriangleRenderer();

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

	private:
		const char* ReadShaderFile(const char* const fileName);
		void CompileShaderText(const char* fileText, const GLenum shaderType, GLuint& shaderId);
	};
}