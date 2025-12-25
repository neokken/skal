//
// Created by okker on 24/12/2025.
//


#pragma once
#include "skal/renderering/renderer.h"
#include "skal/resource/resource_interface.h"

#if 0

namespace skal
{

    class Shader : public IResource
    {
    public:
        Shader(const ResourceUUID& uuid, const std::string& format) : IResource(uuid, format) {}


        RenderShaderHandle GetRenderHandle() const {return m_shaderHandle;}

    protected:
        //bool SaveToFile(const std::string& path) override;

    private:
        friend class ResourceFactory;
        void Load(const std::vector<uint8_t>& data, const std::string& sourcePath);

        RenderShaderHandle m_shaderHandle {};
    };

}

#endif