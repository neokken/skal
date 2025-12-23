//
// Created by okker on 22/12/2025.
//

#include "skal/resource/types/scene_resource.h"

#include "skal/engine.h"
#include "skal/file_io.h"


skal::SceneResource::SceneResource(const ResourceUUID &uuid, const std::string &format) : IResource(uuid, format)
{

}

void skal::SceneResource::Load(const std::vector<uint8_t> &data)
{
    if (data.empty())
    {
        Log::Warn("SceneResource created with empty data");
        m_sceneData = nlohmann::json::object();
        return;
    }

    try
    {
        std::string jsonStr(data.begin(), data.end());
        m_sceneData = nlohmann::json::parse(jsonStr);
    }
    catch (const std::exception &e)
    {
        Log::Error("Failed to parse scene JSON: {}", e.what());
        m_sceneData = nlohmann::json::object();
    }
}


bool skal::SceneResource::SaveToFile(const std::string &path)
{
    try
    {
        std::string jsonStr = m_sceneData.dump(4);
        return skal::Engine.FileIO().WriteTextFile(path, jsonStr);
    } catch (const std::exception &e)
    {
        Log::Error("Failed to save scene: {}", e.what());
        return false;
    }
}

#ifdef SKAL_EDITOR
void skal::SceneResource::UpdateData(const nlohmann::json &data)
{
    m_sceneData = data;
    MarkDirty();
}
#endif
