//
// Created by okker on 20/12/2025.
//

// ReSharper disable CppMemberFunctionMayBeConst
#include "skal/application.h"
#include "skal/device/device.h"
#include "skal/input/input.h"
#include "skal/util/log.h"

namespace skal
{
    Application Engine;
}


void skal::Application::Initialize([[maybe_unused]] const char *projectPath)    // TODO (okke): project path maybe unused
{
    skal::Log::Initialize();
    PrintBanner();
    m_device = new skal::Device(1280, 720); //TODO (okke): hard coded sizes instead of project based.
    m_input = new skal::Input();
}

void skal::Application::Shutdown()
{
    delete m_input;
    delete m_device;
}

void skal::Application::UpdateSystems()
{
    m_device->Update();
    m_input->Update();
}

void skal::Application::RenderScene()
{
    m_device->BeginFrame();
    //TODO (okke):
}

void skal::Application::RenderDebugUI()
{
    //TODO (okke):
}

void skal::Application::PresentFrame()
{
    m_device->EndFrame();
}

void skal::Application::RequestClose()
{
    m_device->RequestClose();
}

bool skal::Application::ShouldRun() const
{
    return !m_device->ShouldClose();
}

void skal::Application::PrintBanner()
{
    Log::Info("Skal ");
}
