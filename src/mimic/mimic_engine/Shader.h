#pragma once
#include "Resource.h"
#include <mimic_render/Shader.h>

#include <memory>

namespace MimicEngine
{
	// #############################################################################
	// Shader stuct (engine):
	// #############################################################################
    struct MimicRender::Shader;

    struct Shader : Resource
    {

    private:
        friend struct ResourceManager;
        friend struct PBRMaterial;

        bool Load(const std::string& path) override;
        unsigned int GetShaderId();
        std::shared_ptr<MimicRender::Shader> _shader;
    };
}