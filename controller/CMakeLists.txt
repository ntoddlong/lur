cmake_minimum_required(VERSION 3.15)
# Generate compile_commands.json
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

project(controller)

set(SOURCES
  src/main.cpp
  src/imgui/imgui_impl_glfw.cpp                           
  src/imgui/imgui_impl_opengl3.cpp
  src/imgui/imgui.cpp
  src/imgui/imgui_demo.cpp
  src/imgui/imgui_draw.cpp
  src/imgui/imgui_tables.cpp
  src/imgui/imgui_widgets.cpp
)

# do I need imgui_internal.h
# or imgui_impl_opengl3_loader.h?

add_executable(controller ${SOURCES})

target_include_directories(controller PUBLIC
  ${PROJECT_SOURCE_DIR}/include/imgui
)

set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
add_subdirectory(glfw-3.3.8)
find_package(OpenGL REQUIRED)
target_link_libraries(controller glfw OpenGL::GL ${CMAKE_DL_LIBS})