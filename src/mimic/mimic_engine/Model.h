#pragma once
#include "Resource.h"
#include <mimic_render/Model.h>

#include <memory>
#include <vector>

namespace MimicEngine
{
    // #############################################################################
    // Model stuct (engine):
    // #############################################################################
    struct MimicRender::Model;
    struct MimicRender::Mesh;

    struct Model : Resource
    {
        bool Load(const std::string& path) override;
        
        std::shared_ptr<MimicRender::Model> _renderModel; // NOTE: move to private
    private:
        friend struct ModelRenderer;

    };
}