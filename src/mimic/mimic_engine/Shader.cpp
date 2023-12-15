#include "Shader.h"

namespace MimicEngine
{
	bool Shader::Load(const std::string& path)
	{
		_shader = MimicRender::Shader::Initialise(path);
		if (_shader == nullptr) return false;

		return true;
	}

	unsigned int Shader::GetShaderId()
	{
		return _shader->_shaderProgramId;
	}
}