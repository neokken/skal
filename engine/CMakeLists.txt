add_subdirectory(extern/glm)

set(SDL_SHARED OFF CACHE BOOL "" FORCE)
set(SDL_STATIC ON CACHE BOOL "" FORCE)
set(SDL_TEST OFF CACHE BOOL "" FORCE)
add_subdirectory(extern/SDL)

set(SPDLOG_BUILD_SHARED OFF CACHE BOOL "" FORCE)
set(SPDLOG_BUILD_EXAMPLE OFF CACHE BOOL "" FORCE)
set(SPDLOG_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(SPDLOG_INSTALL OFF CACHE BOOL "" FORCE)
add_subdirectory(extern/spdlog)

add_subdirectory(extern/fmt)

add_subdirectory(extern/nlohmann)


set(MAGIC_ENUM_OPT_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(MAGIC_ENUM_OPT_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(MAGIC_ENUM_OPT_INSTALL OFF CACHE BOOL "" FORCE)
add_subdirectory(extern/magic_enum)


add_subdirectory(extern/entt)


add_library(glad STATIC extern/glad/src/glad.c)
target_include_directories(glad PUBLIC extern/glad/include)


file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS "src/*.cpp")
file(GLOB_RECURSE HEADERS CONFIGURE_DEPENDS "include/*.h")

list(APPEND SOURCES
        extern/imgui/imgui.cpp
        extern/imgui/imgui_draw.cpp
        extern/imgui/imgui_widgets.cpp
        extern/imgui/imgui_demo.cpp
        extern/imgui/imgui_tables.cpp
        extern/imgui/backends/imgui_impl_sdl3.cpp
        extern/imgui/backends/imgui_impl_opengl3.cpp
)

add_library(SkalEngine STATIC ${SOURCES} ${HEADERS})



# Engine defines HARD CODED FOPR NOW.. TODO (okke): whats the plan with this?
target_compile_definitions(SkalEngine PUBLIC
        SKAL_PLATFORM_WINDOWS
        SKAL_OPENGL
        SKAL_SDL3

        GLM_ENABLE_EXPERIMENTAL
)


target_include_directories(SkalEngine PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
        ${CMAKE_CURRENT_SOURCE_DIR}/extern/stb
        ${CMAKE_CURRENT_SOURCE_DIR}/extern/imgui
        ${CMAKE_CURRENT_SOURCE_DIR}/extern/imgui/backends
)

target_link_libraries(SkalEngine PUBLIC
        glm::glm
        SDL3::SDL3-static
        spdlog::spdlog
        fmt::fmt
        nlohmann_json::nlohmann_json
        magic_enum::magic_enum
        EnTT::EnTT
        glad
)

