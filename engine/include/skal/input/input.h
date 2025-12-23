//
// Created by okker on 20/12/2025.
//

#pragma once

namespace skal::input
{
    /// <summary>
    /// An enum listing all supported keyboard keys.
    /// This uses the same numbering as in SDL3 input, so a SDL-based implementation doesn't need mapping
    /// </summary>
    enum class KeyboardKey
    {
        A = 4,
        B = 5,
        C = 6,
        D = 7,
        E = 8,
        F = 9,
        G = 10,
        H = 11,
        I = 12,
        J = 13,
        K = 14,
        L = 15,
        M = 16,
        N = 17,
        O = 18,
        P = 19,
        Q = 20,
        R = 21,
        S = 22,
        T = 23,
        U = 24,
        V = 25,
        W = 26,
        X = 27,
        Y = 28,
        Z = 29,

        Digit1 = 30,
        Digit2 = 31,
        Digit3 = 32,
        Digit4 = 33,
        Digit5 = 34,
        Digit6 = 35,
        Digit7 = 36,
        Digit8 = 37,
        Digit9 = 38,
        Digit0 = 39,

        Enter = 40,
        Escape = 41,
        Backspace = 42,
        Tab = 43,
        Space = 44,

        Minus = 45,
        Equal = 46,
        LeftBracket = 47,
        RightBracket = 48,
        Backslash = 49,

        Semicolon = 51,
        Apostrophe = 52,
        GraveAccent = 53,
        Comma = 54,
        Period = 55,
        Slash = 56,

        CapsLock = 57,

        F1 = 58,
        F2 = 59,
        F3 = 60,
        F4 = 61,
        F5 = 62,
        F6 = 63,
        F7 = 64,
        F8 = 65,
        F9 = 66,
        F10 = 67,
        F11 = 68,
        F12 = 69,

        PrintScreen = 70,
        ScrollLock = 71,
        Pause = 72,
        Insert = 73,
        Home = 74,
        PageUp = 75,
        Delete = 76,
        End = 77,
        PageDown = 78,
        ArrowRight = 79,
        ArrowLeft = 80,
        ArrowDown = 81,
        ArrowUp = 82,

        NumLock = 83,
        NumpadDivide = 84,
        NumpadMultiply = 85,
        NumpadSubtract = 86,
        NumpadAdd = 87,
        NumpadEnter = 88,
        Numpad1 = 89,
        Numpad2 = 90,
        Numpad3 = 91,
        Numpad4 = 92,
        Numpad5 = 93,
        Numpad6 = 94,
        Numpad7 = 95,
        Numpad8 = 96,
        Numpad9 = 97,
        Numpad0 = 98,
        NumpadDecimal = 99,

        NumpadEqual = 103,
        F13 = 104,
        F14 = 105,
        F15 = 106,
        F16 = 107,
        F17 = 108,
        F18 = 109,
        F19 = 110,
        F20 = 111,
        F21 = 112,
        F22 = 113,
        F23 = 114,
        F24 = 115,

        Menu = 118,

        LeftControl = 224,
        LeftShift = 225,
        LeftAlt = 226,
        LeftSuper = 227,
        RightControl = 228,
        RightShift = 229,
        RightAlt = 230,
        RightSuper = 231
    };


    /// <summary>
    /// <summary>
    /// An enum listing all supported mouse buttons.
    /// This uses the same numbering as in SDL input, SDL doesn't need mapping.
    /// </summary>
    enum class MouseButton
    {
        Left = 0,
        Right = 1,
        Middle = 2
    };
}


// no other platforms yet
#include "skal/platform/sdl3/input/input_sdl3.h"
