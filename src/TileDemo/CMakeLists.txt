file(GLOB_RECURSE SRC "*.cpp" "*.h")
add_executable(TileDemo ${SRC})
target_link_libraries(TileDemo PRIVATE sfml-graphics lib_engine)
finish_ca3_target(TileDemo)
