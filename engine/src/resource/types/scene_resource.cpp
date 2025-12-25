//
// Created by okker on 22/12/2025.
//

#include "skal/resource/types/scene_resource.h"

#include "skal/engine.h"
#include "skal/resource/asset_importer.h"


void skal::SceneResource::Load(const ImportedAsset& asset)
{
    m_sceneData = std::any_cast<nlohmann::json>(asset.payload);
}


#ifdef SKAL_EDITOR


void skal::SceneResource::UpdateData(const nlohmann::json &data)
{
    m_sceneData = data;
    MarkDirty();
}

#endif
