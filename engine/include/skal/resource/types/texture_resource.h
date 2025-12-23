//
// Created by okker on 23/12/2025.
//

#pragma once
#include "skal/renderering/renderer.h"
#include "skal/resource/resource_interface.h"

#include <nlohmann/json.hpp>

namespace skal
{


    // currently Texture file is used for .png type
struct TextureFile
{
    TextureDescriptor::ColorSpace color_space{TextureDescriptor::ColorSpace::Srgb};
    bool gen_mipmaps = false;

    static TextureFile FromJSON(const nlohmann::json& j)
    {
        TextureFile desc;

        const std::string colorSpaceStr = j.value("color_space", "unorm");
        desc.color_space = (colorSpaceStr == "srgb") ? TextureDescriptor::ColorSpace::Srgb : TextureDescriptor::ColorSpace::Unorm;

        desc.gen_mipmaps = j.value("gen_mipmaps", true);

        return desc;
    }

    nlohmann::json ToJSON() const
    {
        nlohmann::json j;
        j["color_space"] = (color_space == TextureDescriptor::ColorSpace::Srgb) ? "srgb" : "unorm";
        j["gen_mipmaps"] = gen_mipmaps;
        return j;
    }
};


class Texture final : public IResource
{
public:
    Texture(const ResourceUUID &uuid, const std::string &format) : IResource(uuid, format) {};
    ~Texture() override;


    [[nodiscard]] ResourceType GetType() const override { return ResourceType::Texture; }

    [[nodiscard]] TextureHandle GetData() const { return m_textureData; }


    void Load(const std::vector<uint8_t>& data, const std::string& sourcePath);


protected:
    //bool SaveToFile(const std::string& path) override;


private:
    TextureHandle m_textureData{TextureHandle::null};
};

}
