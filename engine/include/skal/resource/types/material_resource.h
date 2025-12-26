//
// Created by okker on 26/12/2025.
//


#pragma once
#include "skal/resource/resource_interface.h"


namespace skal
{


    class Material : public IResource
    {
    public:

    private:

        std::unordered_map<std::string, MaterialProperties> m_
        Handle<Shader> m_shader;
    };

}
