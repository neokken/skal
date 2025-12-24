//
// Created by okker on 24/12/2025.
//


#pragma once
#include <optional>

#include "nlohmann/json.hpp"

namespace skal
{
    inline std::optional<nlohmann::json> TryParseJson(const std::vector<uint8_t>& data)
    {
        try {
            return nlohmann::json::parse(data);
        } catch (const nlohmann::json::exception&) {
            return std::nullopt;
        }
    }
}
