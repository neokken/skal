add_library(imgui STATIC
        imgui.cpp
        imgui_draw.cpp
        imgui_widgets.cpp
        imgui_tables.cpp
        imgui_demo.cpp
        backends/imgui_impl_sdl3.cpp
        backends/imgui_impl_opengl3.cpp
)

target_include_directories(imgui PUBLIC . backends)