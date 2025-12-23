//
// Created by okker on 21/12/2025.
//

#pragma once


/*
 * Build Configuration Defines
 *
 * Special defines:
 * - SKAL_EDITOR          : Editor build (has save, import, UI tools)
 *
 *
 *
 * Platform defines:
 * - SKAL_PLATFORM_WINDOWS
 *
 * Rendering backend defines:
 * - SKAL_OPENGL
 *
 * Windowing defines:
 * - SKAL_SDL3
 */


// VERSION
#define SKAL_ENGINE_VERSION "0.0.1"
#define SKAL_VERSION_MAJOR 0
#define SKAL_VERSION_MINOR 0
#define SKAL_VERSION_PATCH 1



// Platform validation
#if !defined(SKAL_PLATFORM_WINDOWS)  // && !defined(other_platform)
    #error "Unsupported platform"
#endif

// Graphics backend validation
#if !defined(SKAL_OPENGL) // && !defined(other_renderer)
    #error "Unsupported graphics backend"
#endif


// Windowing validation
#if !defined(SKAL_SDL3) // && !defined(other windowing thing)
    #error "Unsupported windowing library"
#endif