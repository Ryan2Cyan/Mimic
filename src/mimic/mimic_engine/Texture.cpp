#include "Texture.h"

namespace MimicEngine
{
	bool Texture::Load(const std::string& path)
	{
		_texture = MimicRender::Texture::Initialise(path);
		if (_texture == nullptr) return false;

		return true;
	}

	unsigned int Texture::GetId() const
	{
		if (!_initialised) return 0;
		return _texture->GetId();
	}
}
