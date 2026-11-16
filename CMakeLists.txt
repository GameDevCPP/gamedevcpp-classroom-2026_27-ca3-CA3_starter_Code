cmake_minimum_required(VERSION 3.21)
project(CMakeSFMLProject LANGUAGES CXX)
# Enforce C++17 standard for the entire project
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED YES)
set(CMAKE_CXX_EXTENSIONS NO)


include(FetchContent)
FetchContent_Declare(SFML
    GIT_REPOSITORY https://github.com/SFML/SFML.git
    GIT_TAG 2.6.x)
FetchContent_MakeAvailable(SFML)

# Dear ImGui + ImGui-SFML, optional, wired into the engine loop already.
# Pinned to v1.89: the version imgui-sfml's 2.6.x branch is tested against.
# Newer ImGui changed the texture-ID API and won't compile against it.
FetchContent_Declare(imgui
    URL "https://github.com/ocornut/imgui/archive/v1.89.zip")
FetchContent_MakeAvailable(imgui)

set(IMGUI_DIR ${imgui_SOURCE_DIR})
option(IMGUI_SFML_FIND_SFML "Use find_package to find SFML" OFF)
option(IMGUI_SFML_IMGUI_DEMO "Build imgui_demo.cpp" ON)
add_compile_definitions(IMGUI_SFML_DISABLE_OBSOLETE_FUNCTIONS)
FetchContent_Declare(imgui-sfml
    GIT_REPOSITORY https://github.com/SFML/imgui-sfml.git
    GIT_TAG 2.6.x
    # imgui-sfml 2.6.x (maintenance-only, per its own README) predates SFML's
    # early-2024 U8StringCharTraits change, so sf::Clipboard::getString()'s
    # return type no longer implicitly converts the way this line assumes.
    # Fix via a CMake script doing a literal string replace, not git apply:
    # a context-diff patch is fragile against CRLF checkouts on Windows
    # (core.autocrlf=true), which made "patch does not apply" show up for
    # some students but not others depending on their git config.
    PATCH_COMMAND ${CMAKE_COMMAND} -P ${CMAKE_CURRENT_SOURCE_DIR}/patches/fix-clipboard-u8string.cmake)
FetchContent_MakeAvailable(imgui-sfml)

# B2D - Box2D physics library (optional to use in your game, but the engine's
# system_physics links against it either way)
add_subdirectory("lib/b2d/Box2D")
set(B2D_INCS "lib/b2d/Box2D/")

#### Add Engine and our own library projects####

file(GLOB_RECURSE RESOURCE_FILES "res/*.*")
## Engine ##
file(GLOB_RECURSE SOURCE_FILES engine/*.cpp engine/*.h)
add_library(lib_engine STATIC ${SOURCE_FILES} ${RESOURCE_FILES})
target_include_directories(lib_engine INTERFACE "${CMAKE_SOURCE_DIR}/engine/" PUBLIC SYSTEM ${B2D_INCS})
target_link_libraries(lib_engine lib_tile_level_loader lib_maths lib_ecm Box2D sfml-graphics sfml-audio ImGui-SFML::ImGui-SFML)

## Tile loader lib
file(GLOB_RECURSE SOURCE_FILES lib_tile_level_loader/*.cpp lib_tile_level_loader/*.h)
add_library(lib_tile_level_loader STATIC ${SOURCE_FILES})
target_include_directories(lib_tile_level_loader INTERFACE "${CMAKE_SOURCE_DIR}/lib_tile_level_loader/" )
target_link_libraries(lib_tile_level_loader lib_maths sfml-graphics)

# Maths library
add_library(lib_maths INTERFACE)
target_sources(lib_maths INTERFACE "${CMAKE_SOURCE_DIR}/lib_maths/maths.h")
target_include_directories(lib_maths INTERFACE "${CMAKE_SOURCE_DIR}/lib_maths")
target_link_libraries(lib_maths INTERFACE sfml-system)

## ECM lib
file(GLOB_RECURSE SOURCE_FILES lib_ecm/*.cpp lib_ecm/*.h)
add_library(lib_ecm STATIC ${SOURCE_FILES})
target_include_directories(lib_ecm INTERFACE "${CMAKE_SOURCE_DIR}/lib_ecm" )
target_link_libraries(lib_ecm PRIVATE lib_maths)


add_custom_target(copy_resources ALL COMMAND ${CMAKE_COMMAND}
        -E copy_directory
        "${PROJECT_SOURCE_DIR}/res"
        "${CMAKE_BINARY_DIR}/res"
)

#### Subprojects: each is a real separate CMake project in its own
#### src/<Name>/CMakeLists.txt, pulled in below. Each of those defines its
#### own executable and links it -- that part is what's actually theirs.
#### This function only wires up the plumbing that's identical for all of
#### them and has nothing to do with any one project's own code.
function(finish_ca3_target name)
    target_compile_features(${name} PRIVATE cxx_std_17)
    add_dependencies(${name} copy_resources)
    # add_subdirectory() would otherwise land each binary under
    # build/src/<name>/, away from copy_resources' build/res -- breaking every
    # relative "res/..." path at runtime. Keep all binaries next to res/.
    set_target_properties(${name} PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}")
    install(TARGETS ${name})
    if (WIN32 AND BUILD_SHARED_LIBS)
        add_custom_command(TARGET ${name} POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_RUNTIME_DLLS:${name}> $<TARGET_FILE_DIR:${name}> COMMAND_EXPAND_LISTS)
    endif()
endfunction()

# Your game. Rename the folder (and the project() line in its
# CMakeLists.txt) whenever you like.
add_subdirectory(src/CA3PlaceholderGameTitle)

# Example tool: a small ImGui tile editor. Remove this line and
# src/TileEditor/ if you don't want it.
add_subdirectory(src/TileEditor)

# Example tool: a tiny "playable" room (textured tiles, a drone you move
# with arrow keys, wall collision). Remove this line and src/TileDemo/
# if you don't want it.
add_subdirectory(src/TileDemo)
