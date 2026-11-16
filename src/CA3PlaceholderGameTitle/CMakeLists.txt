file(GLOB_RECURSE SRC "*.cpp" "*.h")
add_executable(CA3PlaceholderGameTitle ${SRC})
target_link_libraries(CA3PlaceholderGameTitle PRIVATE sfml-graphics lib_engine)
finish_ca3_target(CA3PlaceholderGameTitle)
