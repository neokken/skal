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
        SceneResource(const ResourceUUID &uuid, const std::string &format);

        void Load(const std::vector<uint8_t>& data);

        [[nodiscard]] ResourceType GetType() const override { return ResourceType::Scene; }
        [[nodiscard]] const nlohmann::json& GetData() const { return m_sceneData; }

#ifdef SKAL_EDITOR
        void UpdateData(const nlohmann::json& data);
#endif
    protected:
        bool SaveToFile(const std::string& path) override;

    private:
        nlohmann::json m_sceneData;
    };

}