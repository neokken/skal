//
// Created by okker on 22/12/2025.
//

#pragma once

#include "nlohmann/json.hpp"
#include "skal/resource/resource_interface.h"

namespace skal
{

    class SceneResource final : public IResource
    {
    public:
        explicit SceneResource(const ResourceUUID &uuid) : IResource(uuid) {}



        [[nodiscard]] ResourceType GetType() const override { return ResourceType::Scene; }
        [[nodiscard]] const nlohmann::json& GetData() const { return m_sceneData; }

        void Load(const ImportedAsset& asset) override;

#ifdef SKAL_EDITOR
        void UpdateData(const nlohmann::json& data);

        //bool SaveToFile(const std::string& path) override;
#endif

    private:
        friend class ResourceFactory;
        nlohmann::json m_sceneData;
    };

}