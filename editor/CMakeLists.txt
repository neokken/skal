


add_subdirectory(extern/imgui imgui_build)


file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS "src/*.cpp")

list(APPEND SOURCES
        extern/imgui/imgui.cpp
        extern/imgui/imgui_draw.cpp
        extern/imgui/imgui_widgets.cpp
        extern/imgui/imgui_demo.cpp
        extern/imgui/imgui_tables.cpp
        extern/imgui/backends/imgui_impl_sdl3.cpp
        extern/imgui/backends/imgui_impl_opengl3.cpp
)


file(GLOB_RECURSE HEADERS CONFIGURE_DEPENDS "src/*.h")

add_executable(Editor ${SOURCES} ${HEADERS})

target_include_directories(Editor PRIVATE
        src
        extern/imgui
        extern/imgui/backends
)

target_link_libraries(Editor PRIVATE
        SkalEngine
)