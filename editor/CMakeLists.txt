


add_subdirectory(../engine/extern/imgui imgui_build)


file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS "src/*.cpp")


file(GLOB_RECURSE HEADERS CONFIGURE_DEPENDS "src/*.h")

add_executable(Editor ${SOURCES} ${HEADERS})

target_include_directories(Editor PRIVATE
        src
)

target_link_libraries(Editor PRIVATE
        SkalEngine
)