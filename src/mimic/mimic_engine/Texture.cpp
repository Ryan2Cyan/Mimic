#include "Texture.h"
#include <mimic_render/Texture.h>

namespace MimicEngine
{
	bool Texture::Load(const std::string& path)
	{
		_texture = MimicRender::Texture::Initialise(path);
		return _texture != nullptr;
	}

	unsigned int Texture::GetId() const
	{

		if (!_initialised) return 0;
		return _texture->GetId();
	}
}
