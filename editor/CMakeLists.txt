file(GLOB_RECURSE SOURCES "src/*.cpp")
file(GLOB_RECURSE HEADERS "include/*.h")

add_executable(Editor ${SOURCES} ${HEADERS})



target_link_libraries(Editor PRIVATE SkalEngine)