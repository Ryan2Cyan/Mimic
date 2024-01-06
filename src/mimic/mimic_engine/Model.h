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
        
    private:
        friend struct ModelRenderer;
        friend struct MeshCollider;
        friend struct InputHandler;

        std::shared_ptr<MimicRender::Model> _renderModel;
    };
}