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
		GLuint Shader;
		GLuint ProjectionLocation;
		GLuint ViewLocation;
		GLuint ModelLocation;

		void Initialise();
		void Render();
		void Display();
	};
}