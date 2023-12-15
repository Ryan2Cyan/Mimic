#include "Model.h"
#include <mimic_render/Model.h>

namespace MimicEngine
{
	bool Model::Load(const std::string& path)
	{
		_model = MimicRender::Model::Initialise(path);
		if (_model == nullptr) return false;

		return true;
	}
}