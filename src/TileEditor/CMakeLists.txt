file(GLOB_RECURSE SRC "*.cpp" "*.h")
add_executable(TileEditor ${SRC})
target_link_libraries(TileEditor PRIVATE sfml-graphics lib_engine)
finish_ca3_target(TileEditor)
