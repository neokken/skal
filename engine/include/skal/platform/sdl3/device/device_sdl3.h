//
// Created by okker on 20/12/2025.
//

#pragma once

#include <string>

#include "SDL3/SDL_video.h"


class SDL_Window;



struct NativeWindowHandle
{
    void* window;     // SDL_Window*
    void* glContext;  // SDL_GLContext
};


namespace skal
{

    class Device {      //https://github.com/BredaUniversityGames/xs/blob/main/code/sdl3/device_sdl.cpp take a look
    public:
        Device() = delete;
        Device(int width, int height);
        ~Device();

        [[nodiscard]] NativeWindowHandle GetNativeWindowHandle() const;

        void SetTitle(const std::string& title);

        void Update();
        void BeginFrame();
        void EndFrame();


        void RequestClose() { m_shouldClose = true; }
        [[nodiscard]] bool ShouldClose() const { return m_shouldClose; }


        // ReSharper disable once CppMemberFunctionMayBeConst
        [[nodiscard]] SDL_Window* GetWindow() { return m_window; }

        [[nodiscard]] int GetWidth() const;
        [[nodiscard]] int GetHeight() const;

        [[nodiscard]] float GetMonitorUIScale() const;



    private:
        SDL_Window* m_window = nullptr;
        SDL_GLContext m_glContext = nullptr;        // TODO (okke): hard ref to opengL?

        bool m_shouldClose = false;
        int m_height;
        int m_width;
    };

}
