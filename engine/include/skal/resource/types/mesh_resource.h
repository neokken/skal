//
// Created by okker on 23/12/2025.
//

#pragma once
#include "skal/renderering/renderer.h"
#include "skal/resource/resource_interface.h"

namespace skal
{
    class Mesh : public IResource
    {
    public:
        Mesh(const ResourceUUID& uuid, const std::string& format) : IResource(uuid, format){}
        ~Mesh() override;

        [[nodiscard]] ResourceType GetType() const override {return ResourceType::Mesh; }

        void Load(const std::vector<uint8_t>& data);

    private:
        RenderMeshHandle m_meshHandle{};
    };
}
