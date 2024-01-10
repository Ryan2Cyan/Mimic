#pragma once
#include "Resource.h"

namespace MimicRender
{
    struct Shader;
}

namespace MimicEngine
{
    /// <summary>
    /// Wrapper for MimicRender::Shader. Responsible for executing GPU code to render scene objects. See the
    /// Material and ModelRenderer structs.
    /// </summary>
    struct Shader : Resource
    {
    private:
        friend struct ResourceManager;
        friend struct PBRMaterial;
        friend struct ModelRenderer;

        bool Load(const std::string& path) override;
        unsigned int GetShaderId();

        std::shared_ptr<MimicRender::Shader> _renderShader;
    };
}