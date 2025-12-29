//
// Created by okker on 20/12/2025.
//

// ReSharper disable CppMemberFunctionMayBeConst
#include "skal/engine.h"

#include "glm/ext/matrix_clip_space.hpp"
#include "skal/file_io/file_io.h"
#include "skal/device/device.h"
#include "skal/input/input.h"
#include "skal/util/log.h"
#include "skal/ecs/scene.h"
#include "skal/ecs/scene_manager.h"
#include "skal/math/math_constants.h"
#include "skal/renderering/renderer.h"
#include "skal/resource/resource_indexer.h"
#include "skal/resource/resource_manager.h"
#include "skal/util/debug_ui.h"


namespace skal
{
    EngineClass Engine;
}


void skal::EngineClass::Initialize()
{
    skal::Log::Initialize();
    PrintBanner();

    m_fileIO = new skal::FileIO();

    m_device = new skal::Device(1280, 720); //TODO (okke): hard coded sizes instead of project based.
    m_input = new skal::Input();
    m_indexer = new skal::ResourceIndexer();
    m_resources = new skal::ResourceManager();
    m_sceneManager = new skal::SceneManager();
    m_renderer = new skal::Renderer();
}

void skal::EngineClass::LoadProject(const skal::Project &project)
{
    m_project = project;
    m_projectLoaded = true;
    m_device->SetTitle(project.name);

    if (m_project.engine_version != SKAL_ENGINE_VERSION)
    {
        skal::Log::Error("EngineClass::LoadProject - Project was created with engine v{}, running v{}",
                         m_project.engine_version,
                         SKAL_ENGINE_VERSION);
    }

    // Load entry scene

    m_sceneManager->LoadScene(project.entry_scene);
}

void skal::EngineClass::Shutdown()
{
    delete m_sceneManager;

    if (m_debugUI)
    {
        m_debugUI->Shutdown();
    }


    m_resources->GarbageCollect();
    delete m_resources;
    delete m_indexer;
    delete m_input;

    delete m_device;
    delete m_renderer;

    delete m_fileIO;
}

void skal::EngineClass::SetDebugUI(IDebugUI *ui)
{
    if (m_debugUI)
        m_debugUI->Shutdown();

    m_debugUI = ui;

    if (m_debugUI)
        m_debugUI->Initialize();
}

void skal::EngineClass::PreUpdate()
{
    m_device->Update();
    m_input->Update();

    if (!m_projectLoaded || !m_sceneManager->GetActiveScene())
    {
        Log::Critical("EngineClass::PreUpdate - No active scene, cannot continue");
        std::abort();
    }
}

void skal::EngineClass::GameUpdate()
{
    m_sceneManager->GetActiveScene()->Update();
}

void skal::EngineClass::PostUpdate()
{
}

void skal::EngineClass::RenderScene(const FrameData *frameData)
{
    m_device->BeginFrame();

    FrameData final_frame_data;

    if (frameData)
    {
        final_frame_data = *frameData;
    }
    else
    {
        final_frame_data.width = m_device->GetWidth();
        final_frame_data.height = m_device->GetHeight();

        final_frame_data.near_plane = 0.1f;
        final_frame_data.far_plane = 100.f;

        final_frame_data.field_of_view_deg = 45.f;
    }


    m_renderer->BeginFrame(final_frame_data);
}




void skal::EngineClass::PresentFrame()
{
    m_renderer->EndFrame();
    if (m_debugUI) m_debugUI->EndFrame();
    m_device->EndFrame();
}

void skal::EngineClass::RequestClose()
{
    m_device->RequestClose();
}

bool skal::EngineClass::ShouldRun() const
{
    return !m_device->ShouldClose();
}

const skal::Project &skal::EngineClass::GetProject() const
{
    assert(m_projectLoaded && "LoadProject must be called before accessing project");
    return m_project;
}

void skal::EngineClass::PrintBanner()
{
    Log::Info("Skal ");
}
