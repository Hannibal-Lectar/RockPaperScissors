cmake_minimum_required(VERSION 3.16)
project(RockPaperScissors VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()

if(MSVC)
    add_compile_options(/W4 /permissive-)
else()
    add_compile_options(-Wall -Wextra)
endif()

# ---------------------------------------------------------------------
# Dependencies (fetched automatically at configure time)
# ---------------------------------------------------------------------
include(FetchContent)

# --- GLFW: window / input / OpenGL context creation ---
FetchContent_Declare(
    glfw
    GIT_REPOSITORY https://github.com/glfw/glfw.git
    GIT_TAG        3.4
)
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(GLFW_INSTALL OFF CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(glfw)

# --- Dear ImGui: immediate-mode GUI library (source only, no CMake of its own) ---
FetchContent_Declare(
    imgui
    GIT_REPOSITORY https://github.com/ocornut/imgui.git
    GIT_TAG        v1.90.9
)
FetchContent_MakeAvailable(imgui)

add_library(imgui STATIC
    ${imgui_SOURCE_DIR}/imgui.cpp
    ${imgui_SOURCE_DIR}/imgui_draw.cpp
    ${imgui_SOURCE_DIR}/imgui_tables.cpp
    ${imgui_SOURCE_DIR}/imgui_widgets.cpp
    ${imgui_SOURCE_DIR}/imgui_demo.cpp
    ${imgui_SOURCE_DIR}/backends/imgui_impl_glfw.cpp
    ${imgui_SOURCE_DIR}/backends/imgui_impl_opengl3.cpp
)
target_include_directories(imgui PUBLIC
    ${imgui_SOURCE_DIR}
    ${imgui_SOURCE_DIR}/backends
)
target_link_libraries(imgui PUBLIC glfw)

find_package(OpenGL REQUIRED)

# ---------------------------------------------------------------------
# Application target
# ---------------------------------------------------------------------
file(GLOB_RECURSE RPS_SOURCES CONFIGURE_DEPENDS
    ${CMAKE_SOURCE_DIR}/src/*.cpp
)

add_executable(RockPaperScissors ${RPS_SOURCES})

target_include_directories(RockPaperScissors PRIVATE
    ${CMAKE_SOURCE_DIR}/include
)

target_link_libraries(RockPaperScissors PRIVATE imgui OpenGL::GL glfw)

if(APPLE)
    target_link_libraries(RockPaperScissors PRIVATE
        "-framework Cocoa"
        "-framework IOKit"
        "-framework CoreVideo"
    )
endif()

if(WIN32)
    # Keep a console window for diagnostic output; switch to WIN32 (no
    # console) here if you prefer a purely graphical launch.
    set_target_properties(RockPaperScissors PROPERTIES WIN32_EXECUTABLE FALSE)
endif()

# ---------------------------------------------------------------------
# Copy runtime assets/data next to the built executable so the app can
# find its save files and assets regardless of the working directory
# used to launch it.
# ---------------------------------------------------------------------
add_custom_command(TARGET RockPaperScissors POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E make_directory $<TARGET_FILE_DIR:RockPaperScissors>/data
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/data
        $<TARGET_FILE_DIR:RockPaperScissors>/data
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/assets
        $<TARGET_FILE_DIR:RockPaperScissors>/assets
    COMMENT "Copying data/ and assets/ next to the built executable"
)

message(STATUS "Rock Paper Scissors will be built at: ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}")
