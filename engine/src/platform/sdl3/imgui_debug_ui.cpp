//
// Created by okker on 28/12/2025.
//

#include "skal/platform/sdl3/imgui_debug_ui.h"

#include "imgui.h"
#include "imgui_impl_opengl3.h"
#include "imgui_impl_sdl3.h"
#include "skal/platform/sdl3/device/device_sdl3.h"


editor::ImGuiDebugUI::ImGuiDebugUI(const NativeWindowHandle &handle)
{
    m_window = static_cast<SDL_Window*>(handle.window);
    m_glContext = handle.glContext;
}

void editor::ImGuiDebugUI::Initialize()
{
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();

    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
    io.IniFilename = nullptr;

    ImGui_ImplSDL3_InitForOpenGL(m_window, m_glContext);
    ImGui_ImplOpenGL3_Init("#version 450");
}

void editor::ImGuiDebugUI::Shutdown()
{
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplSDL3_Shutdown();
    ImGui::DestroyContext();
}

void editor::ImGuiDebugUI::BeginFrame()
{
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplSDL3_NewFrame();
    ImGui::NewFrame();
}


void editor::ImGuiDebugUI::EndFrame()
{
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void editor::ImGuiDebugUI::ProcessSDLEvent(const SDL_Event &e)
{
    ImGui_ImplSDL3_ProcessEvent(&e);
}
