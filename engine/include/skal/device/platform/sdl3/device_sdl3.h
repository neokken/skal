//
// Created by okker on 20/12/2025.
//

#pragma once



class SDL_Window;

namespace skal
{

    class Device {      //https://github.com/BredaUniversityGames/xs/blob/main/code/sdl3/device_sdl.cpp take a look
    public:
        Device() = delete;
        Device(int width, int height);
        ~Device();

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

        bool m_shouldClose = false;
        int m_height;
        int m_width;
    };

}
