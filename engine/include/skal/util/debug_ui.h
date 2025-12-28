//
// Created by okker on 28/12/2025.
//

#pragma once

#ifdef SKAL_SDL3
#include "SDL3/SDL.h"
#endif

namespace skal
{
    struct IDebugUI
    {
        virtual ~IDebugUI() = default;


        virtual void Initialize() = 0;
        virtual void Shutdown() = 0;

        virtual void BeginFrame() = 0;
        virtual void EndFrame() = 0;

#ifdef SKAL_SDL3
        virtual void ProcessSDLEvent(const SDL_Event& e) = 0;
#endif

    };
}
