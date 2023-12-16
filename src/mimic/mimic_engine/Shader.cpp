#include "Shader.h"

namespace MimicEngine
{
	bool Shader::Load(const std::string& path)
	{
		_renderShader = MimicRender::Shader::Initialise(path);
		if (_renderShader == nullptr) return false;

		return true;
	}

	unsigned int Shader::GetShaderId()
	{
		return _renderShader->_shaderProgramId;
	}
}