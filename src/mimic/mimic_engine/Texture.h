#pragma once
#include "Resource.h"

#include <string>
#include <memory>

namespace MimicRender
{
	struct Texture;
}

namespace MimicEngine
{
	/// <summary>
	/// Wrapper for MimicRender::Texture. Loads 2D textures and stores their ID for rendering access.
	/// </summary>
	struct Texture : Resource
	{
		bool Load(const std::string& path) override;
	private:
		friend struct PBRMaterial;

		unsigned int GetId() const;

		std::shared_ptr<MimicRender::Texture> _texture;
	};
}