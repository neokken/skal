//
// Created by okker on 23/12/2025.
//


#include "skal/resource/types/texture_resource.h"

#include "skal/engine.h"
#include "skal/file_io/file_io.h"

#include "skal/util/json_helper.h"

#if 0
skal::Texture::~Texture()
{
    skal::Engine.Renderer().UnloadTexture(m_textureHandle);
}

void skal::Texture::Load(const std::vector<uint8_t> &data, const std::string &sourcePath)
{
    const auto format = GetFormat();
    if (format == "png")
    {
        const auto json_opt = skal::TryParseJson(data);
        if (!json_opt)
        {
            skal::Log::Error("Texture::Load - Failed to parse JSON");
            return;
        }

        const auto file = TextureFile::Deserialize(*json_opt);

        TextureDescriptor desc{};
        desc.generate_mipmaps = file.gen_mipmaps;
        desc.color_space = file.color_space;


        const std::string imagePath = std::filesystem::path(sourcePath).replace_extension(".png").string();

        if (!Engine.FileIO().Exists(imagePath))
        {
            skal::Log::Error("Texture::Load - texture png, could not find file at {}",imagePath );
            return;
        }

        const std::vector<uint8_t> image = Engine.FileIO().ReadBinaryFile(imagePath);
        m_textureHandle = skal::Engine.Renderer().LoadTexture(GetFormat(), image, desc);
    }
    else
    {
        skal::Log::Error("Texture::Load - Unsupported texture format: {}",format );
    }
}
#endif