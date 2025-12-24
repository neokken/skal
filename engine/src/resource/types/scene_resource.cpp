//
// Created by okker on 22/12/2025.
//

#include "skal/resource/types/scene_resource.h"

#include "skal/engine.h"
#include "skal/file_io/file_io.h"



void skal::SceneResource::Load(const std::vector<uint8_t> &data)
{
    if (data.empty())
    {
        Log::Error("SceneResource::Load - SceneResource created with empty data");
        m_sceneData = nlohmann::json::object();
        return;
    }

    m_sceneData = nlohmann::json::parse(data);
}



#ifdef SKAL_EDITOR
void skal::SceneResource::UpdateData(const nlohmann::json &data)
{
    m_sceneData = data;
    MarkDirty();
}

#endif
