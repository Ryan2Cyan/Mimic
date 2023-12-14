#pragma once
#include "Resource.h"
#include <mimic_render/Texture.h>

namespace MimicEngine
{
	// #############################################################################
	// Texture stuct:
	// #############################################################################
	struct MimicRender::Texture;

	struct Texture : Resource
	{
		bool Load(const std::string& path) override;


		unsigned int GetId() const;

	private:
		std::shared_ptr<MimicRender::Texture> _texture;
	};
}