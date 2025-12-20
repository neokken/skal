file(GLOB_RECURSE SOURCES "src/*.cpp")

add_executable(Player ${SOURCES})

target_link_libraries(Player PRIVATE SkalEngine)