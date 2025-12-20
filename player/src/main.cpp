//
// Created by okker on 20/12/2025.
//

#include <skal/application.h>


int main() {

    skal::Engine.Initialize("./project");

    while (skal::Engine.ShouldRun()) {

        skal::Engine.UpdateSystems();  // Input, physics, game logic

        skal::Engine.RenderScene();
        //editorCtx.RenderGizmos();
        //editorCtx.RenderUI();
        skal::Engine.RenderDebugUI();
        skal::Engine.PresentFrame();

    }

    skal::Engine.Shutdown();
    return 0;
}
