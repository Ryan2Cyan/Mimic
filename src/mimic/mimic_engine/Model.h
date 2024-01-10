#pragma once
#include "Resource.h"

#include <memory>
#include <vector>

namespace MimicRender
{
    struct Model;
}

namespace MimicEngine
{
    /// <summary>
    /// Model: Wrapper for MimicRender::Model. Holds all data relating to a model (including the model's textures and meshes).
    /// </summary>
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