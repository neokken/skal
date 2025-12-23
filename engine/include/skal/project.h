//
// Created by okker on 21/12/2025.
//

#pragma once
#include <nlohmann/json.hpp>
#include <string>

#include "uuid_types.h"
#include "build_config.h"

namespace skal
{
    struct Project
    {
        std::string name{"Default project"};
        std::string engine_version{SKAL_ENGINE_VERSION};

        ResourceUUID entry_scene{};
    };


    inline nlohmann::ordered_json SerializeProject(const Project& project) {
        nlohmann::ordered_json json;
        json["name"] = project.name;
        json["engine_version"] = project.engine_version;
        json["entry_scene"] = project.entry_scene.to_string();
        return json;
    }

    inline Project DeserializeProject(const nlohmann::json& json) {
        Project project;
        project.name = json["name"];
        project.engine_version = json["engine_version"];
        project.entry_scene = ResourceUUID::from_string(json["entry_scene"]);
        return project;
    }


}
