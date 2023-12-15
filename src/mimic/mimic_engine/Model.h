#pragma once
#include "Resource.h"
#include <mimic_render/Model.h>

#include <memory>

namespace MimicEngine
{
    // #############################################################################
    // Model stuct (engine):
    // #############################################################################
    struct MimicRender::Model;

    struct Model : Resource
    {
        bool Load(const std::string& path) override;

    private:
        std::shared_ptr<MimicRender::Model> _model;
    };
}