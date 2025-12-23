//
// Created by okker on 20/12/2025.
//

#include "../../../include/skal/platform/sdl3/input/input_sdl3.h"
#include <skal/input/input.h>
#include <SDL3/SDL.h>


skal::Input::Input()
{
    Update();
}

void skal::Input::Update()
{
    m_prevKeysDown = m_keysDown;
    m_prevMouseDown = m_mouseButtonsDown;

    m_keysDown.clear();

    int numKeys = 0;
    const bool* keyState = SDL_GetKeyboardState(&numKeys);

    for (int i = 0; i < numKeys; ++i) {
        if (keyState[i]) {
            m_keysDown.insert(i);
        }
    }

    float mx, my;
    const Uint32 mouseState = SDL_GetMouseState(&mx, &my);
    m_mousePosition = glm::vec2(mx, my);

    m_mouseButtonsDown.clear();
    if (mouseState & SDL_BUTTON_LMASK) m_mouseButtonsDown.insert(0);
    if (mouseState & SDL_BUTTON_RMASK) m_mouseButtonsDown.insert(1);
    if (mouseState & SDL_BUTTON_MMASK) m_mouseButtonsDown.insert(2);
}

bool skal::Input::GetKeyDown(input::KeyboardKey key) const
{
    int k = static_cast<int>(key);
    return m_keysDown.find(k) != m_keysDown.end();
}

bool skal::Input::GetKeyOnce(input::KeyboardKey key) const
{
    int k = static_cast<int>(key);
    return m_keysDown.find(k) != m_keysDown.end() &&
           m_prevKeysDown.find(k) == m_prevKeysDown.end();
}

bool skal::Input::GetKeyReleased(input::KeyboardKey key) const
{
    int k = static_cast<int>(key);
    return m_prevKeysDown.find(k) != m_prevKeysDown.end() &&
           m_keysDown.find(k) == m_keysDown.end();
}

bool skal::Input::GetMouseButtonDown(input::MouseButton button) const
{
    int b = static_cast<int>(button);
    return m_mouseButtonsDown.find(b) != m_mouseButtonsDown.end();
}

bool skal::Input::GetMouseButtonOnce(input::MouseButton button) const
{
    int b = static_cast<int>(button);
    return m_mouseButtonsDown.find(b) != m_mouseButtonsDown.end() &&
           m_prevMouseDown.find(b) == m_prevMouseDown.end();
}

bool skal::Input::GetMouseButtonReleased(input::MouseButton button) const
{
    int b = static_cast<int>(button);
    return m_prevMouseDown.find(b) != m_prevMouseDown.end() &&
           m_mouseButtonsDown.find(b) == m_mouseButtonsDown.end();
}

glm::vec2 skal::Input::GetMousePosition() const
{
    return m_mousePosition;
}
