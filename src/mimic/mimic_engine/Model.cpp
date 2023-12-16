#include "Model.h"
#include <mimic_render/Model.h>

namespace MimicEngine
{
	bool Model::Load(const std::string& path)
	{
		_renderModel = MimicRender::Model::Initialise(path);
		if (_renderModel == nullptr) return false;

		return true;
	}
}