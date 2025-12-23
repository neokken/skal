//
// Created by okker on 20/12/2025.
//

#pragma once

#include <unordered_set>
#include <glm/glm.hpp>


namespace skal
{
    namespace input
    {
        enum class KeyboardKey;
        enum class MouseButton;

    }

    class Input
    {
    public:
        Input();
        void Update();

        [[nodiscard]] bool GetKeyDown(input::KeyboardKey key) const;
        [[nodiscard]] bool GetKeyOnce(input::KeyboardKey key) const;
        [[nodiscard]] bool GetKeyReleased(input::KeyboardKey key) const;

        [[nodiscard]] bool GetMouseButtonDown(input::MouseButton button) const;
        [[nodiscard]] bool GetMouseButtonOnce(input::MouseButton button) const;
        [[nodiscard]] bool GetMouseButtonReleased(input::MouseButton button) const;


        [[nodiscard]] glm::vec2 GetMousePosition() const;

    private:
        std::unordered_set<int> m_keysDown;
        std::unordered_set<int> m_prevKeysDown;

        std::unordered_set<int> m_mouseButtonsDown;
        std::unordered_set<int> m_prevMouseDown;

        glm::vec2 m_mousePosition{0.0f, 0.0f};
    };
}