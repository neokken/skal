//
// Created by okker on 20/12/2025.
//

#include "skal/device/platform/sdl3/device_sdl3.h"
#include <SDL3/SDL.h>




skal::Device::Device(const int width, const int height) :   // TODO (okke): Should probably take in a settings struct or something
    m_height(height),
    m_width(width)
{
    SDL_Init(SDL_INIT_VIDEO);

    m_window = SDL_CreateWindow(
        "Skal Engine",
        width, height,
        SDL_WINDOW_RESIZABLE | SDL_WINDOW_OPENGL        // TODO (okke): hard reference to opengl?
    );

    m_shouldClose = false;
}

skal::Device::~Device()
{
    if (m_window) {
        SDL_DestroyWindow(m_window);
    }
    SDL_Quit();
}

void skal::Device::Update()
{
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_EVENT_QUIT) {
            m_shouldClose = true;
        }
        else if (event.type == SDL_EVENT_WINDOW_RESIZED) {
            m_width = event.window.data1;
            m_height = event.window.data2;
        }
    }

}

// ReSharper disable once CppMemberFunctionMayBeStatic
void skal::Device::BeginFrame()
{
    // Empty for now, may add timing/profiling later
}

void skal::Device::EndFrame()
{
    SDL_GL_SwapWindow(m_window);
}

int skal::Device::GetWidth() const
{
    int w;
    SDL_GetWindowSize(m_window, &w, nullptr);
    return w;
}

int skal::Device::GetHeight() const
{
    int h;
    SDL_GetWindowSize(m_window, nullptr, &h);
    return h;
}

float skal::Device::GetMonitorUIScale() const
{
    return SDL_GetWindowDisplayScale(m_window);;
}

