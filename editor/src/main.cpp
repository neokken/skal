//
// Created by okker on 20/12/2025.
//

#include <skal/application.h>

#include "skal/input/input.h"
#include "skal/util/log.h"

int main() {

    skal::Engine.Initialize("./project");

    while (skal::Engine.ShouldRun()) {

        skal::Engine.UpdateSystems();  // Input, physics, game logic

        if (skal::Engine.Input().GetKeyDown(skal::input::KeyboardKey::Escape))
        {
            skal::Log::Info("Requesting Shutdown");
            skal::Engine.RequestClose();
        }

        skal::Engine.RenderScene();
        //editorCtx.RenderGizmos();
        //editorCtx.RenderUI();
        skal::Engine.RenderDebugUI();
        skal::Engine.PresentFrame();

    }

    skal::Engine.Shutdown();
    return 0;
}
